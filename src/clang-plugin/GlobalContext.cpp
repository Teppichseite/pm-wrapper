#include "GlobalContext.h"
#include "Types.h"

PointerType GlobalContext::getVariableType(clang::VarDecl *decl) {
  return variables[decl];
}

FunctionType &GlobalContext::getFunctionType(clang::FunctionDecl *decl) {
  return functions[decl];
}

bool GlobalContext::isVariableSet(clang::VarDecl *decl) {
  return variables.count(decl);
}

bool GlobalContext::isFunctionSet(clang::FunctionDecl *decl) {
  return functions.count(decl);
}

void GlobalContext::setVariable(clang::VarDecl *decl, PointerType type) {
  variables[decl] = type;
}

void GlobalContext::setFunctionType(clang::FunctionDecl *decl, FunctionType type) {
  functions[decl] = type;
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
  }
  return "INVALID TYPE";
}

void GlobalContext::printContext() {

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

bool GlobalContext::isSymoblPartOfSource(clang::DeclRefExpr expr){
  
};