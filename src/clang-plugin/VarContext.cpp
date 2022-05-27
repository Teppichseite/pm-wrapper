#include "VarContext.h"

PointerType VarContext::getVariableType(clang::VarDecl *decl) {
  return variables[decl];
}

FunctionType &VarContext::getFunctionType(clang::FunctionDecl *decl) {
  return functions[decl];
}

bool VarContext::isVariableSet(clang::VarDecl *decl) {
  return variables.count(decl);
}

bool VarContext::isFunctionSet(clang::FunctionDecl *decl) {
  return functions.count(decl);
}

void VarContext::setVariable(clang::VarDecl *decl, PointerType type) {
  variables[decl] = type;
}

void VarContext::setFunctionType(clang::FunctionDecl *decl, FunctionType type) {
  functions[decl] = type;
}

void VarContext::printContext() {

  llvm::outs() << "Variables: \n";
  for (auto entry : variables) {
    auto decl = entry.first;
    auto type = entry.second;

    llvm::outs() << decl->getNameAsString() << " -> " << type << "\n";
  }

  llvm::outs() << "\nFunctions: \n";
  for (auto e : functions) {
    auto decl = e.first;
    auto &type = e.second;

    llvm::outs() << decl->getNameAsString() << "(";
    for (auto paramType : type.parameterTypes) {
      llvm::outs() << paramType << ", ";
    }
    llvm::outs() << ") -> " << type.returnType << "\n";
  }
  llvm::outs() << "\n";
}