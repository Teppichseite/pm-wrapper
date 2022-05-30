#include "GlobalEvaluator.h"
#include "ExpressionEvaluator.h"
#include "FunctionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include <vector>

void GlobalEvaluator::run() {
  this->TraverseDecl(context->getTranslationUnitDecl());

  std::vector<PointerType> paramTypes;
  for (int i = 0; i < mainFunction->getNumParams(); i++) {
    paramTypes.push_back(PointerType::NO_PM);
    varContext.setVariable(mainFunction->getParamDecl(i), PointerType::NO_PM);
  }
  varContext.setFunctionType(mainFunction, {.returnType = PointerType::NO_PM,
                                            .parameterTypes = paramTypes});
  FunctionEvaluator functionEvaluator{context, varContext, mainFunction};
  functionEvaluator.run();
  varContext.printContext();
}

bool GlobalEvaluator::VisitFunctionDecl(clang::FunctionDecl *fd) {

  if (fd->isMain() && fd->hasBody()) {
    mainFunction = fd;
    return true;
  }

  auto funcName = fd->getNameAsString();
  if (PM_WRAPPER_FUNCTION_NAMES.find(funcName) !=
      PM_WRAPPER_FUNCTION_NAMES.end()) {
    varContext.setFunctionType(
        fd, {.returnType = PointerType::PM, .reserved = true});
    return true;
  }

  return true;
}

bool GlobalEvaluator::VisitVarDecl(clang::VarDecl *decl) {

  if (decl->isLocalVarDeclOrParm()) {
    return true;
  }

  if (hasPointerTypeAttribute(decl)) {
    varContext.setVariable(decl, getPointerTypeFromAttribute(decl));
  } else {
    varContext.setVariable(decl, PointerType::UNDECLARED);
  }

  if (decl->hasInit()) {
    ExpressionEvaluator evaluator{context, varContext, decl->getInit()};
    auto type = evaluator.run();
    if (!hasPointerTypeAttribute(decl)) {
      varContext.setVariable(decl, type);
    }
    return true;
  }

  return true;
}