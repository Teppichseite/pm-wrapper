#ifndef VAR_CONTEXT_H
#define VAR_CONTEXT_H
#include "Types.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>

struct PluginOptions {
  std::string sourcePath;
};

class GlobalContext {

private:
  std::map<clang::VarDecl *, PointerType> variables;
  std::map<clang::FunctionDecl *, FunctionType> functions;
  PluginOptions options;

public:
  PointerType getVariableType(clang::VarDecl *decl);
  FunctionType &getFunctionType(clang::FunctionDecl *decl);

  bool isVariableSet(clang::VarDecl *decl);
  bool isFunctionSet(clang::FunctionDecl *decl);

  void setVariable(clang::VarDecl *decl, PointerType type);
  void setFunctionType(clang::FunctionDecl *decl, FunctionType type);

  void printContext();

  bool isSymoblPartOfSource(clang::DeclRefExpr expr);
};

#endif