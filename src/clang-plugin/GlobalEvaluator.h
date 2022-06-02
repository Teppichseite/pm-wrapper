#ifndef GLOBAL_EVALUATOR_H
#define GLOBAL_EVALUATOR_H
#include "Types.h"
#include "GlobalContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <array>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <vector>

class GlobalEvaluator : public clang::RecursiveASTVisitor<GlobalEvaluator> {

private:
  clang::ASTContext *context;
  clang::FunctionDecl *mainFunction;
  GlobalContext globalContext;

  std::map<std::string, FunctionType> pmWrapperFunctionTypes;

public:
  explicit GlobalEvaluator(clang::ASTContext *ctx);
  void run();
  bool VisitFunctionDecl(clang::FunctionDecl *fd);
  bool VisitVarDecl(clang::VarDecl *decl);
};

#endif