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

  stream << "((";
  stream << expr->getType().getAsString();
  stream << ")pm_read_object(";
  TraverseStmt(expr);
  stream << "))";
}

void ExpressionWriter::addWriteCall(clang::Expr *pmPtr, clang::Expr *value,
                                    std::function<void(std::string)> writeValue,
                                    std::function<void()> writeOffset,
                                    bool offsetCharSteps) {

  auto pmPtrName = createVarDecl(value, true);
  auto pmPtrValueName = createVarDecl(value, false);
  auto dataValueName = createVarDecl(value, false);

  auto valuePointerType = "(" + value->getType().getAsString() + "*)";

  stream << "(";

  stream << "(" << pmPtrName << " = " << valuePointerType << "("
         << (offsetCharSteps ? "(char*)" : "") << "(";
  TraverseStmt(pmPtr);
  stream << ") + (";
  writeOffset();
  stream << "))),";

  stream << "(" << pmPtrValueName << " = "
         << "*" << valuePointerType << "pm_read_object(" << pmPtrName << ")),";

  stream << "(" << dataValueName << " = (";
  writeValue(pmPtrValueName);
  stream << ")),";

  stream << "pm_write_object(" << pmPtrName << ","
         << "(char*)&" << dataValueName << ","
         << "sizeof(" << value->getType().getAsString() << ")),";

  stream << dataValueName;
  stream << ")";
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

std::string ExpressionWriter::createVarDecl(clang::Expr *expr, bool asPointer) {
  ExpressionWriter writer(context, varManager, expr, ptrTypes, beforeInsertLoc);

  auto varName = varManager.createAdditionalVarName();

  std::stringstream varStream;
  varStream << expr->getType().getAsString();
  if (asPointer) {
    varStream << "*";
  }
  varStream << " ";
  varStream << varName;
  varStream << ";";

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

      llvm::outs() << funcDecl->getNameAsString() << "\n";
      wrapReadCall(arg);
    } else {
      TraverseStmt(arg);
    }

    if (i < numArgs - 1) {
      stream << ",";
    }
  }
  stream << ")";

  return false;
}

bool ExpressionWriter::VisitUnaryOperator(clang::UnaryOperator *op) {
  auto opcode = clang::UnaryOperator::getOpcodeStr(op->getOpcode());

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_AddrOf) {

    {
      auto memberExpr = getSubExpr<clang::MemberExpr>(op->getSubExpr());
      if (memberExpr) {

        auto base = memberExpr->getBase();
        if (getType(base) == PointerType::PM) {

          auto offset = getMemberOffset(memberExpr);

          if (memberExpr->getType()->isArrayType()) {
            stream << "((char*)(";
            TraverseStmt(base);
            stream << ") + (";
            stream << offset;
            stream << "))";
            return false;
          }

          stream << "((";
          stream << memberExpr->getType().getAsString();
          stream << "*)(((char*)(";
          TraverseStmt(base);
          stream << ")) + (";
          stream << offset;
          stream << ")))";
          return false;
        }
      }
    }

    {
      auto subsExpr = getSubExpr<clang::ArraySubscriptExpr>(op->getSubExpr());
      if (subsExpr) {
        auto base = subsExpr->getLHS();
        if (getType(base) == PointerType::PM) {
          stream << "(";
          TraverseStmt(base);
          stream << " + (";
          TraverseStmt(subsExpr->getRHS());
          stream << "))";
          return false;
        }
      }
    }
  }

  if (!op->isIncrementDecrementOp()) {
    stream << opcode.str();
    wrapReadCall(op->getSubExpr());
    return false;
  }

  if (op->isPrefix()) {
    stream << opcode.str();
    TraverseStmt(op->getSubExpr());
    return false;
  }

  TraverseStmt(op->getSubExpr());
  stream << opcode.str();

  return false;
}

bool ExpressionWriter::VisitBinaryOperator(clang::BinaryOperator *op) {

  auto opcode = clang::BinaryOperator::getOpcodeStr(op->getOpcode()).str();

  auto lhs = op->getLHS();
  auto rhs = op->getRHS();

  auto writeValue = [this, rhs, op, &opcode](std::string currentValue) {
    if (op->isAssignmentOp() && !op->isCompoundAssignmentOp()) {
      this->TraverseStmt(rhs);
      return;
    }

    this->stream << currentValue;

    if (op->isCompoundAssignmentOp()) {
      opcode.pop_back();
    }

    stream << opcode;

    this->TraverseStmt(rhs);
  };

  if (op->isAssignmentOp()) {
    auto unaryExpr = getSubExpr<clang::UnaryOperator>(lhs);
    if (unaryExpr &&
        unaryExpr->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref &&
        getType(unaryExpr->getSubExpr()) == PointerType::PM) {
      auto subExpr = unaryExpr->getSubExpr();
      addWriteCall(
          unaryExpr->getSubExpr(), rhs, writeValue,
          [this]() { this->stream << "0"; }, false);
      return false;
    }

    auto memberExpr = getSubExpr<clang::MemberExpr>(lhs);
    if (memberExpr && getType(memberExpr->getBase()) == PointerType::PM) {
      addWriteCall(
          memberExpr->getBase(), rhs, writeValue,
          [this, memberExpr]() { this->stream << getMemberOffset(memberExpr); },
          true);
      return false;
    }

    auto subsExpr = getSubExpr<clang::ArraySubscriptExpr>(lhs);
    if (subsExpr && getType(subsExpr->getLHS()) == PointerType::PM) {
      auto base = subsExpr->getLHS();
      addWriteCall(
          base, lhs, writeValue,
          [this, subsExpr]() { this->TraverseStmt(subsExpr->getRHS()); },
          false);
      return false;
    }
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

  stream << "(" << expr->getType().getAsString() << ")";
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
      stream << ",";
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

    stream << "(" << itemType.getAsString() << "*)"
           << "((char*)(";
    TraverseStmt(expr->getBase());
    stream << ") + " << getMemberOffset(expr) << ")";
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
  stream << "(";
  TraverseStmt(expr->getSubExpr());
  stream << ")";
  return false;
}

bool ExpressionWriter::VisitUnaryExprOrTypeTraitExpr(
    clang::UnaryExprOrTypeTraitExpr *expr) {

  stream << clang::getTraitSpelling(expr->getKind());
  stream << "(";
  if (expr->isArgumentType()) {
    stream << expr->getArgumentType().getAsString();
  } else {
    TraverseStmt(expr->getArgumentExpr());
  }
  stream << ")";

  return false;
}