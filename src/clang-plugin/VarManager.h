#ifndef VAR_CONTEXT_H
#define VAR_CONTEXT_H
#include "Types.h"
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>

struct PluginOptions {
  std::string sourcePath;
};

class VarManager {

private:
  clang::ASTContext &context;
  std::map<clang::VarDecl *, PointerType> variables;
  std::map<clang::FunctionDecl *, FunctionType> functions;
  PluginOptions &options;

public:
  VarManager(clang::ASTContext &context, PluginOptions &options)
      : context(context), options(options) {}

  PointerType getVariableType(clang::VarDecl *decl);
  FunctionType &getFunctionType(clang::FunctionDecl *decl);

  bool isVariableSet(clang::VarDecl *decl);
  bool isFunctionSet(clang::FunctionDecl *decl);

  void setVariable(clang::VarDecl *decl, PointerType type);
  void setFunctionType(clang::FunctionDecl *decl, FunctionType type);

  void print();

  bool isSymbolPartOfSource(clang::Decl *decl);

  void registerVariable(clang::FunctionDecl *funcDecl, clang::VarDecl *decl);
};

#endif