#include "VarManager.h"
#include "ExpressionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Type.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <experimental/filesystem>
#include <llvm-13/llvm/Support/raw_ostream.h>

PointerType VarManager::getVariableType(clang::VarDecl *decl) {
  return variables[decl];
}

FunctionType &VarManager::getFunctionType(clang::FunctionDecl *decl) {
  return functions[decl];
}

bool VarManager::isVariableSet(clang::VarDecl *decl) {
  return variables.count(decl);
}

bool VarManager::isFunctionSet(clang::FunctionDecl *decl) {
  return functions.count(decl);
}

void VarManager::setVariable(clang::VarDecl *decl, PointerType type) {
  variables[decl] = type;
}

void VarManager::setFunctionType(clang::FunctionDecl *decl, FunctionType type) {
  functions[decl] = type;
  if (decl->getNameAsString() == "pm_read_object") {
    pmWrapperReadObjectDecl = decl;
    return;
  }

  if (decl->getNameAsString() == "pm_write_object") {
    pmWrapperWriteObjectDecl = decl;
    return;
  }
}

std::string pointerTypeToString(PointerType type) {
  switch (type) {
  case PM:
    return "PM";
  case NO_PM:
    return "NO PM";
  case UNDECLARED:
    return "UNDECLARED";
  case UNKNOWN:
    return "UNKNWON";
  case NULL_PTR:
    return "NULL";
  }
  return "INVALID TYPE";
}

void VarManager::print() {
  llvm::outs() << "\nOptions: \n";
  llvm::outs() << "Source path: " << options.sourcePath << "\n\n";

  llvm::outs() << "Variables: \n";
  for (auto entry : variables) {
    auto decl = entry.first;
    auto type = entry.second;

    llvm::outs() << decl->getNameAsString() << " -> "
                 << pointerTypeToString(type) << "\n";
  }

  llvm::outs() << "\nFunctions: \n";
  for (auto e : functions) {
    auto decl = e.first;
    auto &type = e.second;

    llvm::outs() << decl->getNameAsString() << "(";
    for (auto paramType : type.parameterTypes) {
      llvm::outs() << pointerTypeToString(paramType) << ", ";
    }
    llvm::outs() << ") -> " << pointerTypeToString(type.returnType) << "\n";
  }
  llvm::outs() << "\n";
}

using path = std::experimental::filesystem::path;

bool isPathSubPath(path &parentPath, path &subPath) {
  for (auto p = parentPath.begin(), s = subPath.begin(); p != parentPath.end();
       ++p, ++s) {

    if (s == subPath.end() || *p != *s) {
      return false;
    }
  }
  return true;
}

bool VarManager::isSymbolPartOfSource(clang::Decl *decl) {
  auto &manager = context.getSourceManager();
  path relativeSymbolPath{manager.getFilename(decl->getLocation()).str()};
  auto absoluteSymbolPath =
      std::experimental::filesystem::canonical(relativeSymbolPath);
  auto absoluteSourcePath =
      std::experimental::filesystem::canonical(options.sourcePath);
  return isPathSubPath(absoluteSourcePath, absoluteSymbolPath);
};

void VarManager::registerVariable(clang::FunctionDecl *funcDecl,
                                  clang::VarDecl *decl) {

  if (!isSymbolPartOfSource(decl)) {
    return;
  }

  if (hasPointerTypeAttribute(decl)) {
    setVariable(decl, getPointerTypeFromAttribute(decl));
  } else {
    setVariable(decl, PointerType::UNDECLARED);
  }

  if (decl->hasInit()) {
    ExpressionEvaluator evaluator{context, *this, funcDecl, decl->getInit()};
    auto type = evaluator.run();
    if (!hasPointerTypeAttribute(decl)) {
      setVariable(decl, type);
    }
  }
}

clang::CallExpr *VarManager::CreatePmReadCallExpr(clang::Expr *arg) {

  auto declRef = clang::DeclRefExpr::Create(
      context, {}, {}, pmWrapperReadObjectDecl, false, clang::SourceLocation{},
      pmWrapperReadObjectDecl->getType(), clang::ExprValueKind::VK_LValue);

  auto callExpr = clang::CallExpr::Create(
      context, declRef, {arg}, pmWrapperReadObjectDecl->getReturnType(),
      clang::ExprValueKind::VK_LValue, {}, {});

  return callExpr;
};

PointerType VarManager::GetUpdatedPointerType(PointerType oldType,
                                              PointerType newType) {
  return PointerType::PM;
};
