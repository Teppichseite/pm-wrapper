#include "GlobalEvaluator.h"
#include "ExpressionEvaluator.h"
#include "FunctionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include <vector>

GlobalEvaluator::GlobalEvaluator(clang::ASTContext *ctx) : context{ctx} {
  pmWrapperFunctionTypes = {
      {"pm_root", {.returnType = PointerType::PM, .parameterTypes = {}}},
      {"pm_root_reg",
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
       {.returnType = PointerType::NO_PM,
        .parameterTypes = {PointerType::PM}}}};
}

void GlobalEvaluator::run() {
  this->TraverseDecl(context->getTranslationUnitDecl());

  std::vector<PointerType> paramTypes;
  for (int i = 0; i < mainFunction->getNumParams(); i++) {
    paramTypes.push_back(PointerType::NO_PM);
    globalContext.setVariable(mainFunction->getParamDecl(i), PointerType::NO_PM);
  }
  globalContext.setFunctionType(mainFunction, {.returnType = PointerType::NO_PM,
                                            .parameterTypes = paramTypes});
  FunctionEvaluator functionEvaluator{context, globalContext, mainFunction};
  functionEvaluator.run();
  globalContext.printContext();
}

bool GlobalEvaluator::VisitFunctionDecl(clang::FunctionDecl *fd) {

  if (fd->isMain() && fd->hasBody()) {
    mainFunction = fd;
    return true;
  }

  auto funcName = fd->getNameAsString();
  if (pmWrapperFunctionTypes.find(funcName) != pmWrapperFunctionTypes.end()) {
    globalContext.setFunctionType(fd, pmWrapperFunctionTypes[funcName]);
    return true;
  }

  return true;
}

bool GlobalEvaluator::VisitVarDecl(clang::VarDecl *decl) {

  if (decl->isLocalVarDeclOrParm()) {
    return true;
  }

  if (hasPointerTypeAttribute(decl)) {
    globalContext.setVariable(decl, getPointerTypeFromAttribute(decl));
  } else {
    globalContext.setVariable(decl, PointerType::UNDECLARED);
  }

  if (decl->hasInit()) {
    ExpressionEvaluator evaluator{context, globalContext, decl->getInit()};
    auto type = evaluator.run();
    if (!hasPointerTypeAttribute(decl)) {
      globalContext.setVariable(decl, type);
    }
    return true;
  }

  return true;
}