#include "GlobalEvaluator.h"
#include "ExpressionEvaluator.h"
#include "FunctionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include "VarManager.h"
#include <clang/AST/Decl.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <memory>
#include <vector>

GlobalEvaluator::GlobalEvaluator(clang::ASTContext &ctx, VarManager &manager)
    : context{ctx}, varManager(manager) {
  pmWrapperFunctionTypes = {
      {"pm_get_root", {.returnType = PointerType::PM, .parameterTypes = {}}},
      {"pm_get_root_reg",
       {.returnType = PointerType::PM, .parameterTypes = {PointerType::NO_PM}}},
      {"pm_alloc",
       {.returnType = PointerType::PM, .parameterTypes = {PointerType::NO_PM}}},
      {"pm_alloc_reg",
       {.returnType = PointerType::PM,
        .parameterTypes = {PointerType::NO_PM, PointerType::NO_PM}}},
      {"pm_calloc",
       {.returnType = PointerType::PM, .parameterTypes = {PointerType::NO_PM}}},
      {"pm_calloc_reg",
       {.returnType = PointerType::PM,
        .parameterTypes = {PointerType::NO_PM, PointerType::NO_PM}}},
      {"pm_read_object",
       {.returnType = PointerType::NO_PM, .parameterTypes = {PointerType::PM}}},
      {"pm_write_object",
       {.returnType = PointerType::NO_PM,
        .parameterTypes = {PointerType::PM, PointerType::NO_PM,
                           PointerType::NO_PM}}}};
}

void GlobalEvaluator::run() {
  this->TraverseDecl(context.getTranslationUnitDecl());
  std::vector<PointerType> paramTypes;
  for (int i = 0; i < mainFunction->getNumParams(); i++) {
    paramTypes.push_back(PointerType::NO_PM);
    varManager.setVariable(mainFunction->getParamDecl(i), PointerType::NO_PM);
  }
  varManager.setFunctionType(mainFunction, {.returnType = PointerType::NO_PM,
                                            .parameterTypes = paramTypes});

  FunctionEvaluator functionEvaluator{context, varManager, mainFunction};
  functionEvaluator.run();

  evaluateUncalledFunctions();
}

void GlobalEvaluator::evaluateUncalledFunctions() {

  for (auto pair : functionRefCounts) {
    auto decl = pair.first;
    auto count = pair.second;
    if (count.callCount <= 0) {
      if (varManager.isFunctionSet(decl)) {
        continue;
      }

      uncalledFunctions.push(decl);
    }
  }

  while (!uncalledFunctions.empty()) {
    auto uncalledFunc = uncalledFunctions.top();
    uncalledFunctions.pop();

    llvm::outs() << uncalledFunc->getNameAsString() << "\n";

    if (!hasPointerTypeAttribute(uncalledFunc) &&
        uncalledFunc->getReturnType()->isPointerType()) {
      varManager.reportError(
          uncalledFunc->getSourceRange(),
          "Please specify the pointer type for the return value for "
          "dynamically called functions");
    }

    std::vector<PointerType> uncalledFunParamTypes;
    for (int i = 0; i < uncalledFunc->getNumParams(); i++) {

      auto paramDecl = uncalledFunc->getParamDecl(i);
      if (!hasPointerTypeAttribute(paramDecl) &&
          paramDecl->getType()->isPointerType()) {
        varManager.reportError(
            paramDecl->getSourceRange(),
            "Please specify the pointer type for a parameter for "
            "dynamically called functions");
        continue;
      }

      auto paramType = paramDecl->getType()->isPointerType()
                           ? getPointerTypeFromAttribute(paramDecl)
                           : PointerType::NO_PM;
      uncalledFunParamTypes.push_back(paramType);
      varManager.setVariable(paramDecl, paramType);
    }

    auto uncalledFuncType = uncalledFunc->getReturnType()->isPointerType()
                                ? getPointerTypeFromAttribute(uncalledFunc)
                                : PointerType::NO_PM;

    varManager.setFunctionType(uncalledFunc,
                               {.returnType = uncalledFuncType,
                                .parameterTypes = uncalledFunParamTypes});

    FunctionEvaluator functionEvaluator{context, varManager, uncalledFunc};
    functionEvaluator.run();
  }
}

bool GlobalEvaluator::VisitFunctionDecl(clang::FunctionDecl *fd) {

  if (fd->isMain() && fd->hasBody()) {
    mainFunction = fd;
    return true;
  }

  auto funcName = fd->getNameAsString();
  if (pmWrapperFunctionTypes.find(funcName) != pmWrapperFunctionTypes.end()) {
    varManager.setFunctionType(fd, pmWrapperFunctionTypes[funcName]);
    return true;
  }

  return true;
}

bool GlobalEvaluator::VisitVarDecl(clang::VarDecl *decl) {
  if (decl->isLocalVarDeclOrParm()) {
    return true;
  }
  varManager.registerVariable(nullptr, decl);
  return true;
}

bool GlobalEvaluator::VisitDeclRefExpr(clang::DeclRefExpr *expr) {

  if (!expr->getDecl()->getType()->isFunctionType()) {
    return true;
  }

  if (!varManager.isSymbolPartOfSource(expr->getDecl())) {
    return true;
  }

  auto functionDecl = llvm::dyn_cast<clang::FunctionDecl>(expr->getDecl());
  if (!functionDecl) {
    return true;
  }

  functionRefCounts[functionDecl].refCount++;

  return true;
}

bool GlobalEvaluator::VisitCallExpr(clang::CallExpr *expr) {

  auto decl = expr->getDirectCallee();
  if (decl == nullptr) {
    return true;
  }

  if (!varManager.isSymbolPartOfSource(decl)) {
    return true;
  }

  functionRefCounts[decl].callCount++;

  return true;
}