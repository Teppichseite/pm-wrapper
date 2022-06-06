#include "FunctionEvaluator.h"
#include "ExpressionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include <algorithm>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ParentMapContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Type.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/raw_ostream.h>

PointerType FunctionEvaluator::run() {
  resultingPointerType = PointerType::UNKNOWN;
  this->TraverseDecl(functionDecl);

  if (resultingPointerType == PointerType::NULL_PTR) {
    varManager.reportError(
        functionDecl->getSourceRange(),
        "Cannot return NULL when pointer type cannot be deduced.");
  }

  return resultingPointerType;
}

bool FunctionEvaluator::VisitVarDecl(clang::VarDecl *decl) {
  if (llvm::isa<clang::ParmVarDecl>(decl)) {
    return true;
  }
  varManager.registerVariable(functionDecl, decl);
  return true;
}

bool FunctionEvaluator::VisitExpr(clang::Expr *expr) {
  const auto &parents = context.getParents(*expr);

  if (parents.size() <= 0) {
    return true;
  }

  auto stmt = parents[0].get<clang::Stmt>();
  if (!stmt) {
    return true;
  }

  if (llvm::isa<clang::ReturnStmt>(stmt)) {
    return true;
  };

  if (!llvm::isa<clang::Expr>(stmt)) {
    ExpressionEvaluator evaluator{context, varManager, functionDecl, expr};
    evaluator.run();
  }

  return true;
}

bool FunctionEvaluator::VisitReturnStmt(clang::ReturnStmt *stmt) {
  if (functionDecl->getReturnType()->isVoidType()) {
    resultingPointerType = PointerType::NO_PM;
    return true;
  }

  ExpressionEvaluator evaluator{context, varManager, functionDecl,
                                stmt->getRetValue()};
  auto newResultingPointerType = evaluator.run();

  if (hasPointerTypeAttribute(functionDecl)) {
    resultingPointerType = getPointerTypeFromAttribute(functionDecl);
    return true;
  }

  if (newResultingPointerType == PointerType::NULL_PTR &&
      resultingPointerType != PointerType::UNKNOWN) {
    return true;
  }

  resultingPointerType = newResultingPointerType;
  return true;
}