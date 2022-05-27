#ifndef VAR_CONTEXT_H
#define VAR_CONTEXT_H
#include "Types.h"
#include <clang/AST/Decl.h>

class VarContext {

private:
  std::map<clang::VarDecl *, PointerType> variables;
  std::map<clang::FunctionDecl *, FunctionType> functions;

public:
  PointerType getVariableType(clang::VarDecl *decl);
  FunctionType &getFunctionType(clang::FunctionDecl *decl);

  bool isVariableSet(clang::VarDecl *decl);
  bool isFunctionSet(clang::FunctionDecl *decl);

  void setVariable(clang::VarDecl *decl, PointerType type);
  void setFunctionType(clang::FunctionDecl *decl, FunctionType type);

  void printContext();
};

#endif