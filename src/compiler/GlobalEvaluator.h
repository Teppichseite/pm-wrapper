#ifndef GLOBAL_EVALUATOR_H
#define GLOBAL_EVALUATOR_H
#include "Types.h"
#include "VarManager.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <array>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <map>
#include <stack>
#include <vector>

class GlobalEvaluator : public clang::RecursiveASTVisitor<GlobalEvaluator> {

private:
  struct FunctionRefCounter {
    int refCount;
    int callCount;
  };

  clang::ASTContext &context;
  VarManager &varManager;
  clang::FunctionDecl *mainFunction;

  std::map<std::string, FunctionType> pmWrapperFunctionTypes;

  std::map<clang::FunctionDecl *, FunctionRefCounter> functionRefCounts;
  std::stack<clang::FunctionDecl *> uncalledFunctions;
  void evaluateUncalledFunctions();

public:
  explicit GlobalEvaluator(clang::ASTContext &ctx, VarManager &varManager);
  void run();
  bool VisitFunctionDecl(clang::FunctionDecl *fd);
  bool VisitVarDecl(clang::VarDecl *decl);
  bool VisitDeclRefExpr(clang::DeclRefExpr *expr);
  bool VisitCallExpr(clang::CallExpr *expr);
};

#endif