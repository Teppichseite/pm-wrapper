#ifndef VAR_CONTEXT_H
#define VAR_CONTEXT_H
#include "Types.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <string>

struct PluginOptions {
  std::string sourcePath;
};

class VarManager {

private:
  clang::ASTContext &context;
  std::map<clang::VarDecl *, PointerType> variables;
  std::map<clang::FunctionDecl *, FunctionType> functions;
  PluginOptions &options;
  clang::Rewriter &rewriter;

  clang::FunctionDecl *pmWrapperReadObjectDecl;
  clang::FunctionDecl *pmWrapperWriteObjectDecl;

  std::string additionalVarPostfix;
  int additionalVarCount;

public:
  VarManager(clang::ASTContext &context, PluginOptions &options,
             clang::Rewriter &rewriter);

  PointerType getVariableType(clang::VarDecl *decl);
  FunctionType &getFunctionType(clang::FunctionDecl *decl);

  bool isVariableSet(clang::VarDecl *decl);
  bool isFunctionSet(clang::FunctionDecl *decl);

  void setVariable(clang::VarDecl *decl, PointerType type);
  void setFunctionType(clang::FunctionDecl *decl, FunctionType type);

  void printPretty();
  void print();

  bool isSymbolPartOfSource(clang::Decl *decl);

  void registerVariable(clang::FunctionDecl *funcDecl, clang::VarDecl *decl);

  clang::CallExpr *CreatePmReadCallExpr(clang::Expr *ptr);
  clang::CallExpr *CreatePmWriteCallExpr(clang::Expr *pmPtr, clang::Expr *vmPtr,
                                         clang::Expr *size);

  void reportDiagnostic(clang::DiagnosticsEngine::Level level,
                        clang::SourceRange range, std::string message);

  void reportError(clang::SourceRange range, std::string message);
  void reportWarning(clang::SourceRange range, std::string message);

  clang::Rewriter &getRewriter();

  std::string createAdditionalVarName();
};

#endif