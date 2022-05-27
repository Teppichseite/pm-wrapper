#include "GlobalEvaluator.h"
#include "FunctionEvaluator.h"
#include "Types.h"

void GlobalEvaluator::run() {
  this->TraverseDecl(context->getTranslationUnitDecl());
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