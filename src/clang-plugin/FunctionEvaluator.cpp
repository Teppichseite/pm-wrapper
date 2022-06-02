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
  resultingPointerType = PointerType::NO_PM;
  this->TraverseDecl(functionDecl);
  return resultingPointerType;
}

bool FunctionEvaluator::VisitVarDecl(clang::VarDecl *decl) {

  if (llvm::isa<clang::ParmVarDecl>(decl)) {
    return true;
  }

  if (hasPointerTypeAttribute(decl)) {
    globalContext.setVariable(decl, getPointerTypeFromAttribute(decl));
  } else {
    globalContext.setVariable(decl, PointerType::UNDECLARED);
  }

  if (decl->hasInit()) {
    ExpressionEvaluator evaluator{context, globalContext, decl->getInit()};
    auto type = evaluator.run();
    if (!hasPointerTypeAttribute(decl)) {
      globalContext.setVariable(decl, type);
    }
    return true;
  }

  return true;
}

bool FunctionEvaluator::VisitExpr(clang::Expr *expr) {
  const auto &parents = context->getParents(*expr);

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
    ExpressionEvaluator evaluator{context, globalContext, expr};
    evaluator.run();
  }

  return true;
}

bool FunctionEvaluator::VisitReturnStmt(clang::ReturnStmt *stmt) {
  if (functionDecl->getReturnType()->isVoidType()) {
    return true;
  }

  ExpressionEvaluator evaluator{context, globalContext, stmt->getRetValue()};
  resultingPointerType = evaluator.run();

  if (hasPointerTypeAttribute(functionDecl)) {
    resultingPointerType = getPointerTypeFromAttribute(functionDecl);
  }
  return true;
}