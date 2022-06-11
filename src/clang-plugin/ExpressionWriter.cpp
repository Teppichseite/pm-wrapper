#include "ExpressionWriter.h"
#include "Types.h"
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
#include <llvm-13/llvm/ADT/APInt.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <string>
#include <string_view>
#include <vector>

void ExpressionWriter::run() {
  varManager.getRewriter().ReplaceText(expression->getSourceRange(),
                                       evaluateToString());

  for (auto decl : varDecls) {
    varManager.getRewriter().InsertText(expression->getBeginLoc(), decl);
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

bool ExpressionWriter::VisitDeclRefExpr(clang::DeclRefExpr *expr) {
  stream << expr->getDecl()->getNameAsString();
  return false;
}

bool ExpressionWriter::VisitCallExpr(clang::CallExpr *expr) {

  stream << expr->getDirectCallee()->getNameAsString();

  stream << "(";
  auto numArgs = expr->getNumArgs();
  for (int i = 0; i < numArgs; i++) {
    auto arg = expr->getArg(i);
    TraverseStmt(arg);
    if (i < numArgs - 1) {
      stream << ",";
    }
  }
  stream << ")";

  return false;
}

bool ExpressionWriter::VisitUnaryOperator(clang::UnaryOperator *op) {
  auto opcode = clang::UnaryOperator::getOpcodeStr(op->getOpcode());

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

std::string ExpressionWriter::createVarDecl(clang::Expr *expr) {
  ExpressionWriter writer(context, varManager, expr, ptrTypes);
  std::stringstream varStream;
  varStream << expr->getType().getAsString();
  varStream << " _addr1 = ";
  varStream << writer.evaluateToString();
  varStream << "; ";

  varDecls.push_back(varStream.str());

  return "_addr1";
}

bool ExpressionWriter::VisitBinaryOperator(clang::BinaryOperator *op) {

  auto lhs = op->getLHS();
  auto rhs = op->getRHS();

  {
    auto unaryExpr = getSubExpr<clang::UnaryOperator>(lhs);
    if (unaryExpr) {
      if (unaryExpr->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref) {
        auto subExpr = unaryExpr->getSubExpr();
        if (getType(subExpr) == PointerType::PM) {
          stream << "pm_write_object(";
          TraverseStmt(subExpr);
          stream << ",";
          wrapReadCall(subExpr);
          stream << ",";
          TraverseStmt(rhs);
          stream << ",sizeof(";
          stream << unaryExpr->getType().getAsString();
          stream << "))";
          return false;
        }
      }
    }
  }

  {
    auto memberExpr = getSubExpr<clang::MemberExpr>(lhs);
    if (memberExpr) {
      auto base = memberExpr->getBase();
      if (getType(base) == PointerType::PM) {

        auto varName = createVarDecl(rhs);

        stream << "pm_write_object(";
        TraverseStmt(base);
        stream << ",&(";
        TraverseStmt(memberExpr);
        stream << "),&";
        stream << varName;
        stream << ",sizeof(";
        stream << memberExpr->getType().getAsString();
        stream << "))";
        return false;
      }
    }
  }

  {
    auto subsExpr = getSubExpr<clang::ArraySubscriptExpr>(lhs);
    if (subsExpr) {
      auto base = subsExpr->getLHS();
      if (getType(base) == PointerType::PM) {
        stream << "pm_write_object(";
        TraverseStmt(base);
        stream << ",&(";
        TraverseStmt(subsExpr);
        stream << "),";
        TraverseStmt(rhs);
        stream << ",sizeof(";
        stream << subsExpr->getType().getAsString();
        stream << "))";
        return false;
      }
    }
  }

  auto opcode = clang::BinaryOperator::getOpcodeStr(op->getOpcode());
  TraverseStmt(op->getLHS());
  stream << opcode.str();
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
  wrapReadCall(expr->getLHS());
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