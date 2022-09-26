#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "Types.h"
#include "VarManager.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>

class FunctionEvaluator : public clang::RecursiveASTVisitor<FunctionEvaluator> {

private:
  clang::ASTContext &context;
  VarManager &varManager;
  clang::FunctionDecl *functionDecl;
  PointerType resultingPointerType;

public:
  explicit FunctionEvaluator(clang::ASTContext &context, VarManager &varManager,
                             clang::FunctionDecl *decl)
      : context(context), varManager(varManager), functionDecl(decl){};
  PointerType run();
  bool VisitVarDecl(clang::VarDecl *decl);
  bool VisitExpr(clang::Expr *expr);
  bool VisitReturnStmt(clang::ReturnStmt *returnStmt);
};

#endif