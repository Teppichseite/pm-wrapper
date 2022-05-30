#include "ExpressionEvaluator.h"
#include "FunctionEvaluator.h"
#include "Types.h"
#include "Util.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/OperationKinds.h>
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
  setType(expr, varContext.getVariableType(varDecl));

  return false;
}

bool ExpressionEvaluator::VisitCallExpr(clang::CallExpr *expr) {

  auto decl = expr->getDirectCallee();

  if (varContext.isFunctionSet(decl)) {
    auto &funcType = varContext.getFunctionType(decl);
    setType(expr, funcType.returnType);
  } else {

    auto args = expr->getArgs();

    std::vector<PointerType> paramTypes;
    auto paramCount = decl->getNumParams();
    for (int i = 0; i < paramCount; i++) {
      auto argType = getType(args[i]);
      paramTypes.push_back(argType);
      varContext.setVariable(decl->getParamDecl(i), argType);
    }

    FunctionEvaluator functionEvaluator{context, varContext, decl};
    auto resultType = functionEvaluator.run();

    setType(expr, resultType);
    varContext.setFunctionType(
        decl, {.returnType = resultType, .parameterTypes = paramTypes});
  }

  return false;
}

bool ExpressionEvaluator::VisitUnaryOperator(clang::UnaryOperator *op) {

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_AddrOf) {
    auto qualType = op->getSubExpr();
    setType(op, PointerType::NO_PM);
    return false;
  }

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref) {

    if (getType(op->getSubExpr()) != PointerType::PM) {
      setType(op, PointerType::NO_PM);
      return false;
    }

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

    if (lType == PointerType::UNDECLARED) {
      auto refExpr = getLastRefExpr();
      clang::VarDecl *varDecl =
          llvm::dyn_cast<clang::VarDecl>(refExpr->getDecl());
      PointerType targetType = varContext.getVariableType(varDecl);
      varContext.setVariable(varDecl, rType);
      setType(op, rType);
      return false;
    }

    if (lType != rType) {
      reportError(context, op->getOperatorLoc(),
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
  auto qualType = expr->getType();
  auto subPointerType = getType(expr->getSubExpr());

  if (subPointerType == PointerType::PM && !qualType->isPointerType()) {
    setType(expr, PointerType::NO_PM);
    reportWarning(
        context, expr->getBeginLoc(),
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
    reportWarning(context, expr->getBeginLoc(),
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
  setType(expr, getType(expr->getBase()));
  return false;
}