#include "VarContext.h"
#include "Types.h"

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

std::string pointerTypeToString(PointerType type) {
  switch (type) {
  case PM:
    return "PM";
  case VM:
    return "VM";
  case UNDECLARED:
    return "UNDECLARED";
  case UNKNOWN:
    return "UNKNWON";
  case NO_POINTER:
    return "NO_POINTER";
  }
  return "INVALID_TYPE";
}

void VarContext::printContext() {

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