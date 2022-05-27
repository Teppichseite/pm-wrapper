#include "ExpressionEvaluator.h"
#include "FunctionEvaluator.h"
#include "Types.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
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
    setType(expr, PointerType::VM);
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
    FunctionEvaluator functionEvaluator{context, varContext, decl};

    auto args = expr->getArgs();

    std::vector<PointerType> paramTypes;
    auto paramCount = decl->getNumParams();
    for (int i = 0; i < paramCount; i++) {
      auto argType = getType(args[i]);
      paramTypes.push_back(argType);
      varContext.setVariable(decl->getParamDecl(i), argType);
    }

    auto resultType = functionEvaluator.run();
    setType(expr, resultType);
    varContext.setFunctionType(
        decl, {.returnType = resultType, .parameterTypes = paramTypes});
  }

  return false;
}

bool ExpressionEvaluator::VisitUnaryOperator(clang::UnaryOperator *op) {

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_AddrOf) {
    setType(op, PointerType::VM);
    return false;
  }

  if (op->getOpcode() == clang::UnaryOperator::Opcode::UO_Deref) {
    setType(op, PointerType::NO_POINTER);
    return false;
  }

  return false;
}

bool ExpressionEvaluator::VisitBinaryOperator(clang::BinaryOperator *op) {
  PointerType lType = getType(op->getLHS());
  PointerType rType = getType(op->getRHS());

  if (op->getOpcode() == clang::BinaryOperator::Opcode::BO_Assign) {

    if (lType != rType) {
      llvm::outs() << "Assign mismatch"
                   << "\n";
      return false;
    }

    if (lType == PointerType::UNDECLARED) {
      auto refExpr = getLastRefExpr();
      clang::VarDecl *varDecl =
          llvm::dyn_cast<clang::VarDecl>(refExpr->getDecl());
      PointerType targetType = varContext.getVariableType(varDecl);
      varContext.setVariable(varDecl, rType);
      setType(op, rType);
      return false;
    }
  }

  setType(op, lType);
  return false;
}

bool ExpressionEvaluator::VisitIntegerLiteral(clang::IntegerLiteral *literal) {
  setType(literal, PointerType::NO_POINTER);
  return false;
}

bool ExpressionEvaluator::VisitFloatingLiteral(
    clang::FloatingLiteral *literal) {
  setType(literal, PointerType::NO_POINTER);
  return false;
}

bool ExpressionEvaluator::VisitStringLiteral(clang::StringLiteral *literal) {
  setType(literal, PointerType::NO_POINTER);
  return false;
};

bool ExpressionEvaluator::VisitCharacterLiteral(
    clang::CharacterLiteral *literal) {
  setType(literal, PointerType::NO_POINTER);
  return false;
};

bool ExpressionEvaluator::VisitCastExpr(clang::CastExpr *expr) {
  setType(expr, getType(expr->getSubExpr()));
  return false;
};