#ifndef FUNCTION_EVALUATOR_H
#define FUNCTION_EVALUATOR_H
#include "Types.h"
#include "VarContext.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>

class FunctionEvaluator : public clang::RecursiveASTVisitor<FunctionEvaluator> {

private:
  clang::ASTContext *context;
  clang::FunctionDecl *functionDecl;
  VarContext &varContext;
  PointerType resultingPointerType;

public:
  explicit FunctionEvaluator(clang::ASTContext *context, VarContext &varContext,
                             clang::FunctionDecl *decl)
      : context(context), varContext(varContext), functionDecl(decl){};
  PointerType run();
  bool VisitVarDecl(clang::VarDecl *decl);
  bool VisitExpr(clang::Expr *expr);
  bool VisitReturnStmt(clang::ReturnStmt *returnStmt);
};

#endif