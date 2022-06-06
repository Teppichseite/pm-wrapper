#include "ExpressionEvaluator.h"
#include "FunctionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/OperationKinds.h>
#include <clang/AST/Type.h>
#include <clang/Basic/AttrKinds.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <vector>

// Collect expressions in stack
class ExpressionCollector
    : public clang::RecursiveASTVisitor<ExpressionCollector> {

private:
  std::stack<clang::Expr *> expressions;

public:
  ExpressionCollector(clang::Expr *expr) { this->TraverseStmt(expr); }

  std::stack<clang::Expr *> &getExpressionStack() { return expressions; }

  bool VisitExpr(clang::Expr *expr) {
    expressions.push(expr);
    return true;
  }
};

PointerType ExpressionEvaluator::run() {

  ExpressionCollector collector{expression};
  auto &expressions = collector.getExpressionStack();

  while (true) {
    if (expressions.size() <= 0) {
      break;
    }

    clang::Expr *expression = expressions.top();
    expressions.pop();

    this->TraverseStmt(expression);
  }

  return currentPointerType;
}

void ExpressionEvaluator::setType(clang::Expr *expr, PointerType type) {
  ptrTypes[expr] = type;
  currentPointerType = type;

  clang::DeclRefExpr *refExpr = llvm::dyn_cast<clang::DeclRefExpr>(expr);
  if (refExpr) {
    lastRefExpressions.push(refExpr);
  }
}

PointerType ExpressionEvaluator::getType(clang::Expr *expr) {
  return ptrTypes[expr];
}

clang::DeclRefExpr *ExpressionEvaluator::getLastRefExpr() {
  clang::DeclRefExpr *expr = lastRefExpressions.top();
  lastRefExpressions.pop();
  return expr;
}

bool ExpressionEvaluator::VisitDeclRefExpr(clang::DeclRefExpr *expr) {

  if (expr->getDecl()->getType()->isFunctionType()) {
    setType(expr, PointerType::NO_PM);
    return false;
  }

  clang::VarDecl *varDecl = llvm::dyn_cast<clang::VarDecl>(expr->getDecl());
  if (!varDecl) {
    return false;
  }

  if (!varManager.isSymbolPartOfSource(varDecl)) {
    setType(expr, PointerType::NO_PM);
    return false;
  }

  setType(expr, varManager.getVariableType(varDecl));
  return false;
}

bool ExpressionEvaluator::VisitCallExpr(clang::CallExpr *expr) {

  auto decl = expr->getDirectCallee();
  if (decl == nullptr) {
    setType(expr, PointerType::UNKNOWN);
    return false;
  }

  if (varManager.isFunctionSet(decl)) {
    auto &funcType = varManager.getFunctionType(decl);
    setType(expr, funcType.returnType);
    return false;
  }

  if (!varManager.isSymbolPartOfSource(decl)) {
    setType(expr, PointerType::NO_PM);
    return false;
  }

  if (decl == function) {
    if (!decl->getReturnType()->isPointerType()) {
      setType(expr, PointerType::NO_PM);
      return false;
    }

    if (!hasPointerTypeAttribute(decl)) {
      varManager.reportError(
          decl->getSourceRange(),
          "Recursive functions which return a pointer type are required to "
          "have a pointer type attribute specified");
      return false;
    }

    setType(expr, getPointerTypeFromAttribute(decl));
    return false;
  }

  auto args = expr->getArgs();
  std::vector<PointerType> paramTypes;
  auto paramCount = decl->getNumParams();
  for (int i = 0; i < paramCount; i++) {
    auto argType = getType(args[i]);
    paramTypes.push_back(argType);

    auto paramDecl = decl->getParamDecl(i);
    if (hasPointerTypeAttribute(paramDecl)) {
      auto alreadyDeclaredType = getPointerTypeFromAttribute(paramDecl);
      if (alreadyDeclaredType != argType) {
        varManager.reportError(
            args[i]->getSourceRange(),
            "Declared function parameter pointer type does not match "
            "pointer type of argument");
      }
    }
    varManager.setVariable(decl->getParamDecl(i), argType);
  }

  FunctionEvaluator functionEvaluator{context, varManager, decl};
  auto resultType = functionEvaluator.run();

  setType(expr, resultType);
  varManager.setFunctionType(
      decl, {.returnType = resultType, .parameterTypes = paramTypes});

  return false;
}

bool ExpressionEvaluator::VisitUnaryOperator(clang::UnaryOperator *op) {

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_AddrOf) {

    auto memberExpr = llvm::dyn_cast<clang::MemberExpr>(op->getSubExpr());
    if (memberExpr) {
      if (getType(memberExpr->getBase()) == PointerType::PM) {
        setType(op, PointerType::PM);
        return false;
      }

      setType(op, PointerType::NO_PM);
      return false;
    }

    auto arraySubsExpr =
        llvm::dyn_cast<clang::ArraySubscriptExpr>(op->getSubExpr());
    if (arraySubsExpr) {
      if (getType(arraySubsExpr->getLHS()) == PointerType::PM) {
        setType(op, PointerType::PM);
        return false;
      }

      setType(op, PointerType::NO_PM);
      return false;
    }

    setType(op, PointerType::NO_PM);
    return false;
  }

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref) {

    if (getType(op->getSubExpr()) != PointerType::PM) {
      setType(op, PointerType::NO_PM);
      return false;
    }

    op->setSubExpr(varManager.CreatePmReadCallExpr(op->getSubExpr()));

    auto qualType = op->getType();
    if (!qualType->isPointerType()) {
      setType(op, PointerType::NO_PM);
      return false;
    }

    setType(op, PointerType::PM);
    return false;
  }

  setType(op, getType(op));
  return false;
}

bool ExpressionEvaluator::VisitBinaryOperator(clang::BinaryOperator *op) {
  PointerType lType = getType(op->getLHS());
  PointerType rType = getType(op->getRHS());

  if (op->getOpcode() == clang::BinaryOperator::Opcode::BO_Assign) {

    if (lType == PointerType::UNDECLARED || lType == PointerType::NULL_PTR) {
      auto refExpr = getLastRefExpr();
      clang::VarDecl *varDecl =
          llvm::dyn_cast<clang::VarDecl>(refExpr->getDecl());
      PointerType targetType = varManager.getVariableType(varDecl);
      varManager.setVariable(varDecl, rType);
      setType(op, rType);
      return false;
    }

    if (rType == NULL_PTR) {
      setType(op, lType);
      return false;
    }

    if (lType != rType) {
      varManager.reportError(
          op->getSourceRange(),
          "Left hand side pointer type does not match right hand side "
          "pointer type");
      setType(op, lType);
      return false;
    }
  }

  setType(op, lType);
  return false;
}

bool ExpressionEvaluator::VisitIntegerLiteral(clang::IntegerLiteral *literal) {
  setType(literal, PointerType::NO_PM);
  return false;
}

bool ExpressionEvaluator::VisitFloatingLiteral(
    clang::FloatingLiteral *literal) {
  setType(literal, PointerType::NO_PM);
  return false;
}

bool ExpressionEvaluator::VisitStringLiteral(clang::StringLiteral *literal) {
  setType(literal, PointerType::NO_PM);
  return false;
};

bool ExpressionEvaluator::VisitCharacterLiteral(
    clang::CharacterLiteral *literal) {
  setType(literal, PointerType::NO_PM);
  return false;
};

bool ExpressionEvaluator::VisitCastExpr(clang::CastExpr *expr) {

  if (expr->getCastKind() == clang::CastKind::CK_NullToPointer) {
    setType(expr, PointerType::NULL_PTR);
    return false;
  }

  auto qualType = expr->getType();
  auto subPointerType = getType(expr->getSubExpr());

  if (subPointerType == PointerType::PM && !qualType->isPointerType()) {
    setType(expr, PointerType::NO_PM);
    varManager.reportWarning(
        expr->getSourceRange(),
        "Casting a PM pointer to a non pointer value should be avoided");
    return false;
  }

  setType(expr, getType(expr->getSubExpr()));
  return false;
};

bool ExpressionEvaluator::VisitArraySubscriptExpr(
    clang::ArraySubscriptExpr *expr) {

  auto pointerType = getType(expr->getLHS());
  auto qualType = expr->getType();

  if (pointerType == PointerType::PM) {
    if (qualType->isPointerType()) {
      setType(expr, getType(expr->getLHS()));
      return false;
    }

    setType(expr, NO_PM);
    return false;
  }

  if (qualType->isPointerType()) {
    varManager.reportWarning(
        expr->getSourceRange(),
        "Assumed VM pointer for this variable. Use the Pointer Type "
        "Attribute in case it is incorrect.");
  }

  setType(expr, getType(expr->getLHS()));
  return false;
};

bool ExpressionEvaluator::VisitInitListExpr(clang::InitListExpr *expr) {
  setType(expr, PointerType::NO_PM);
  return false;
};

bool ExpressionEvaluator::VisitMemberExpr(clang::MemberExpr *expr) {

  auto pointerType = getType(expr->getBase());
  auto qualType = expr->getType();

  if (pointerType == PointerType::PM) {
    if (qualType->isPointerType() || qualType->isArrayType()) {
      setType(expr, PointerType::PM);
      return false;
    }

    setType(expr, PointerType::NO_PM);
    return false;
  }

  if (qualType->isPointerType()) {
    varManager.reportWarning(
        expr->getSourceRange(),
        "Assumed VM pointer for this variable. Use the Pointer Type "
        "Attribute in case it is incorrect.");
  }

  setType(expr, getType(expr->getBase()));
  return false;
}

bool ExpressionEvaluator::VisitParenExpr(clang::ParenExpr *expr) {
  setType(expr, getType(expr->getSubExpr()));
  return false;
}