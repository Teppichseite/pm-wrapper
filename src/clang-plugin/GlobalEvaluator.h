#ifndef GLOBAL_EVALUATOR_H
#define GLOBAL_EVALUATOR_H
#include "Types.h"
#include "VarContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <array>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <vector>

class GlobalEvaluator : public clang::RecursiveASTVisitor<GlobalEvaluator> {

private:
  clang::ASTContext *context;
  clang::FunctionDecl *mainFunction;
  VarContext varContext;

  const std::set<std::string> PM_WRAPPER_FUNCTION_NAMES = {
      "pm_root",      "pm_root_reg", "pm_alloc",
      "pm_alloc_reg", "pm_calloc",   "pm_calloc_reg"};

public:
  explicit GlobalEvaluator(clang::ASTContext *ctx) : context{ctx} {};
  void run();
  bool VisitFunctionDecl(clang::FunctionDecl *fd);
  bool VisitVarDecl(clang::VarDecl *decl);
};

#endif