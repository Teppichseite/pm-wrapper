#include "ExpressionWriter.h"
#include "Types.h"
#include "clang/AST/RecordLayout.h"
#include "clang/Lex/Lexer.h"
#include <algorithm>
#include <array>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/OperationKinds.h>
#include <clang/AST/ParentMapContext.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/StmtIterator.h>
#include <clang/AST/Type.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/TypeTraits.h>
#include <functional>
#include <llvm-13/llvm/ADT/APInt.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <string>
#include <string_view>
#include <vector>

#define O_PAR "("
#define C_PAR ")"
#define COMMA ","
#define PAR(expr) O_PAR << expr << C_PAR
#define APP(expr) (stream << expr, "")
#define N_APP(expr) PAR(((expr), ""))
#define CAST(type, value) PAR(PAR(type) PAR(value))
#define CALL(name, args) name << PAR(args)
#define ASSIGN(lhs, rhs) PAR(lhs << " = " << rhs)

#define READ(args) CALL("pm_read_object", args)
#define WRITE(args) CALL("pm_write_object", args)

void ExpressionWriter::run() {

  clang::SourceLocation startLoc =
      varManager.getRewriter().getSourceMgr().getFileLoc(
          expression->getBeginLoc());
  clang::SourceLocation endLoc =
      varManager.getRewriter().getSourceMgr().getFileLoc(
          expression->getEndLoc());

  varManager.getRewriter().ReplaceText(clang::SourceRange{startLoc, endLoc},
                                       evaluateToString());

  for (auto decl : varDecls) {
    varManager.getRewriter().InsertText(beforeInsertLoc, decl);
  }
}

std::string ExpressionWriter::evaluateToString() {
  this->TraverseStmt(expression);
  return stream.str();
};

template <class X> X *getSubExpr(clang::Expr *expr) {
  auto parenExpr = llvm::dyn_cast<clang::ParenExpr>(expr);
  if (parenExpr) {
    return getSubExpr<X>(parenExpr->getSubExpr());
  }

  auto castExpr = llvm::dyn_cast<clang::CastExpr>(expr);
  if (castExpr) {
    return getSubExpr<X>(castExpr->getSubExpr());
  }

  return llvm::dyn_cast<X>(expr);
}

PointerType ExpressionWriter::getType(clang::Expr *expr) {
  return ptrTypes[expr];
}

std::string exprToString(clang::Expr *expr) {
  clang::LangOptions lo;
  std::string out_str;
  llvm::raw_string_ostream outstream(out_str);
  expr->printPretty(outstream, NULL, clang::PrintingPolicy(lo));
  return out_str;
}

void ExpressionWriter::wrapReadCall(clang::Expr *expr) {
  if (getType(expr) != PointerType::PM) {
    TraverseStmt(expr);
    return;
  }

  stream << PAR(PAR(expr->getType().getAsString())
                << READ(N_APP(TraverseStmt(expr))));
}

void ExpressionWriter::addWriteCall(clang::Expr *pmPtr,
                                    std::function<void()> writeOffset,
                                    clang::QualType valueType,
                                    std::function<void(std::string)> writeValue,
                                    bool offsetCharSteps, bool returnOldValue) {

  auto pmPtrName = createVarDecl(valueType, true);
  auto pmPtrValueName = createVarDecl(valueType, false);
  auto dataValueName = createVarDecl(valueType, false);

  auto valuePointerType = valueType.getAsString();
  stream << PAR(
      ASSIGN(pmPtrName, CAST(valuePointerType << "*",
                             PAR((offsetCharSteps ? APP(PAR("char*")) : "")
                                 << N_APP(TraverseStmt(pmPtr)))
                                 << " + " << N_APP(writeOffset())))
      << COMMA
      << ASSIGN(pmPtrValueName,
                "*" << CAST(valuePointerType << "*", READ(pmPtrName)))
      << COMMA << ASSIGN(dataValueName, N_APP(writeValue(pmPtrValueName)))
      << COMMA
      << WRITE(pmPtrName << COMMA << CAST("char*", "&" << dataValueName)
                         << COMMA << CALL("sizeof", valueType.getAsString()))
      << COMMA << (returnOldValue ? pmPtrValueName : dataValueName));
};

bool ExpressionWriter::addAssignment(clang::Expr *lhs, clang::QualType rhsType,
                                     std::function<void()> writeRhs,
                                     bool isCompoundAssignment,
                                     std::string opcode, bool returnOldValue) {
  auto writeValue = [&](std::string currentValue) {
    if (!isCompoundAssignment) {
      writeRhs();
      return;
    }

    this->stream << currentValue;

    opcode.pop_back();

    stream << opcode;

    writeRhs();
  };

  auto unaryExpr = getSubExpr<clang::UnaryOperator>(lhs);
  if (unaryExpr &&
      unaryExpr->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref &&
      getType(unaryExpr->getSubExpr()) == PointerType::PM) {
    auto subExpr = unaryExpr->getSubExpr();
    addWriteCall(
        unaryExpr->getSubExpr(), [&]() { this->stream << "0"; }, rhsType,
        writeValue, false, returnOldValue);
    return true;
  }

  auto memberExpr = getSubExpr<clang::MemberExpr>(lhs);
  if (memberExpr && getType(memberExpr->getBase()) == PointerType::PM) {
    addWriteCall(
        memberExpr->getBase(),
        [&]() { this->stream << getMemberOffset(memberExpr); }, rhsType,
        writeValue, true, returnOldValue);

    return true;
  }

  auto subsExpr = getSubExpr<clang::ArraySubscriptExpr>(lhs);
  if (subsExpr && getType(subsExpr->getLHS()) == PointerType::PM) {
    addWriteCall(
        subsExpr->getLHS(), [&]() { this->TraverseStmt(subsExpr->getRHS()); },
        rhsType, writeValue, false, returnOldValue);
    return true;
  }

  return false;
};

uint64_t ExpressionWriter::getMemberOffset(clang::MemberExpr *expr) {
  auto fieldDecl = llvm::dyn_cast<clang::FieldDecl>(expr->getMemberDecl());

  if (!fieldDecl) {
    return -1;
  }

  auto recordDecl = fieldDecl->getParent();
  auto &recordLayout = context.getASTRecordLayout(recordDecl);
  return recordLayout.getFieldOffset(fieldDecl->getFieldIndex()) / 8;
};

std::string ExpressionWriter::createVarDecl(clang::QualType type,
                                            bool applyPointer) {
  auto varName = varManager.createAdditionalVarName();

  std::stringstream varStream;
  varStream << type.getAsString() << (applyPointer ? "* " : " ") << varName
            << ";";

  varDecls.push_back(varStream.str());

  return varName;
}

bool ExpressionWriter::VisitDeclRefExpr(clang::DeclRefExpr *expr) {
  stream << expr->getDecl()->getNameAsString();
  return false;
}

bool ExpressionWriter::VisitCallExpr(clang::CallExpr *expr) {
  TraverseStmt(expr->getCallee());
  stream << "(";
  auto numArgs = expr->getNumArgs();
  for (int i = 0; i < numArgs; i++) {
    auto arg = expr->getArg(i);

    auto funcDecl = expr->getDirectCallee();
    if (funcDecl && (!varManager.isSymbolPartOfSource(funcDecl) &&
                     !varManager.isFunctionSet(funcDecl))) {
      wrapReadCall(arg);
    } else {
      TraverseStmt(arg);
    }

    if (i < numArgs - 1) {
      stream << COMMA;
    }
  }
  stream << ")";

  return false;
}

bool ExpressionWriter::VisitUnaryOperator(clang::UnaryOperator *op) {
  auto opcode = clang::UnaryOperator::getOpcodeStr(op->getOpcode()).str();

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_AddrOf) {

    {
      auto memberExpr = getSubExpr<clang::MemberExpr>(op->getSubExpr());
      if (memberExpr) {

        auto base = memberExpr->getBase();
        if (getType(base) == PointerType::PM) {

          auto offset = getMemberOffset(memberExpr);

          if (memberExpr->getType()->isArrayType()) {
            TraverseStmt(memberExpr);
            return false;
          }

          stream << CAST(memberExpr->getType().getAsString() << "*",
                         CAST("char*", N_APP(TraverseStmt(base)))
                             << "+" << offset);
          return false;
        }
      }
    }

    auto subsExpr = getSubExpr<clang::ArraySubscriptExpr>(op->getSubExpr());
    if (subsExpr && getType(subsExpr->getLHS()) == PointerType::PM) {
      if (getType(subsExpr->getLHS()) == PointerType::PM) {
        stream << PAR(N_APP(TraverseStmt(subsExpr->getLHS()))
                      << "+" << N_APP(TraverseStmt(subsExpr->getRHS())));
        return false;
      }
    }
  }

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref) {
    stream << opcode;
    wrapReadCall(op->getSubExpr());
    return false;
  }

  if (op->isIncrementDecrementOp() &&
      addAssignment(
          op->getSubExpr(), op->getSubExpr()->getType(),
          [&]() { stream << "1"; }, true, opcode, op->isPostfix())) {

    return false;
  }

  if (op->isPostfix()) {
    TraverseStmt(op->getSubExpr());
    stream << opcode;
    return false;
  }

  stream << opcode;
  TraverseStmt(op->getSubExpr());

  return false;
}

bool ExpressionWriter::VisitBinaryOperator(clang::BinaryOperator *op) {

  auto opcode = clang::BinaryOperator::getOpcodeStr(op->getOpcode()).str();

  auto lhs = op->getLHS();
  auto rhs = op->getRHS();

  if (op->isAssignmentOp() &&
      addAssignment(
          lhs, rhs->getType(), [&]() { TraverseStmt(op->getRHS()); },
          op->isCompoundAssignmentOp(), opcode, false)) {
    return false;
  }

  TraverseStmt(op->getLHS());
  stream << opcode;
  TraverseStmt(op->getRHS());
  return false;
}

bool ExpressionWriter::VisitIntegerLiteral(clang::IntegerLiteral *literal) {
  stream << exprToString(literal);
  return false;
}

bool ExpressionWriter::VisitFloatingLiteral(clang::FloatingLiteral *literal) {
  stream << exprToString(literal);
  return false;
}

bool ExpressionWriter::VisitStringLiteral(clang::StringLiteral *literal) {
  stream << exprToString(literal);
  return false;
};

bool ExpressionWriter::VisitCharacterLiteral(clang::CharacterLiteral *literal) {
  stream << exprToString(literal);
  return false;
};

bool ExpressionWriter::VisitCastExpr(clang::CastExpr *expr) {

  if (expr->getCastKind() == clang::CastKind::CK_NullToPointer) {
    stream << "NULL";
    return false;
  }

  if (llvm::isa<clang::ImplicitCastExpr>(expr)) {
    TraverseStmt(expr->getSubExpr());
    return false;
  }

  stream << PAR(expr->getType().getAsString());
  TraverseStmt(expr->getSubExpr());
  return false;
};

bool ExpressionWriter::VisitArraySubscriptExpr(
    clang::ArraySubscriptExpr *expr) {

  if (expr->getLHS()->getType()->isArrayType()) {
    TraverseStmt(expr->getLHS());
  } else {
    wrapReadCall(expr->getLHS());
  }

  stream << "[";
  TraverseStmt(expr->getRHS());
  stream << "]";
  return false;
};

bool ExpressionWriter::VisitInitListExpr(clang::InitListExpr *expr) {
  stream << "{";
  auto numInits = expr->getNumInits();
  for (int i = 0; i < numInits; i++) {
    auto arg = expr->getInit(i);
    TraverseStmt(arg);
    if (i < numInits - 1) {
      stream << COMMA;
    }
  }
  stream << "}";
  return false;
};

bool ExpressionWriter::VisitDesignatedInitExpr(
    clang::DesignatedInitExpr *expr) {

  auto desigs = expr->designators();
  auto numDesigs = desigs.size();
  int subExprIndex = 1;
  for (auto &desig : desigs) {

    if (desig.isArrayDesignator()) {
      stream << "[";
      TraverseStmt(expr->getSubExpr(subExprIndex));
      stream << "]";
      subExprIndex++;
      continue;
    }

    if (desig.isArrayRangeDesignator()) {
      stream << "[";
      TraverseStmt(expr->getSubExpr(subExprIndex));
      stream << " ... ";
      TraverseStmt(expr->getSubExpr(subExprIndex + 1));
      stream << "]";
      subExprIndex += 2;
      continue;
    }

    stream << ".";
    stream << desig.getFieldName()->getName().str();
  }
  stream << "=";
  TraverseStmt(expr->getInit());
  return false;
}

bool ExpressionWriter::VisitMemberExpr(clang::MemberExpr *expr) {

  if (getType(expr->getBase()) == PointerType::PM &&
      expr->getType()->isArrayType()) {

    auto itemType = expr->getType()->getAsArrayTypeUnsafe()->getElementType();

    stream << CAST(itemType.getAsString() << "*",
                   CAST("char*", N_APP(TraverseStmt(expr->getBase())))
                       << "+" << getMemberOffset(expr));
    return false;
  }

  wrapReadCall(expr->getBase());

  if (expr->isArrow()) {
    stream << "->";
  } else {
    stream << ".";
  }

  stream << expr->getMemberNameInfo().getAsString();
  return false;
}

bool ExpressionWriter::VisitParenExpr(clang::ParenExpr *expr) {
  stream << PAR(N_APP(TraverseStmt(expr->getSubExpr())));
  return false;
}

bool ExpressionWriter::VisitUnaryExprOrTypeTraitExpr(
    clang::UnaryExprOrTypeTraitExpr *expr) {

  stream << clang::getTraitSpelling(expr->getKind()) << O_PAR;
  if (expr->isArgumentType()) {
    stream << expr->getArgumentType().getAsString();
  } else {
    TraverseStmt(expr->getArgumentExpr());
  }
  stream << C_PAR;

  return false;
}