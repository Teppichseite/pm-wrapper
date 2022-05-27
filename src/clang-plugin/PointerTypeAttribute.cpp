#include "PointerTypeAttribute.h"
#include "Types.h"
#include <clang/AST/Attrs.inc>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Expr.h>
#include <clang/AST/PrettyPrinter.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/raw_ostream.h>

clang::AnnotateAttr *getPointerTypeAttribute(clang::Decl *decl) {
  auto &attrs = decl->getAttrs();
  for (auto attr : attrs) {

    clang::AnnotateAttr *annotateAttr =
        llvm::dyn_cast<clang::AnnotateAttr>(attr);

    if (!annotateAttr) {
      continue;
    }

    if (annotateAttr->getAnnotation() == "pointer_type") {
      return annotateAttr;
    }
  }

  return nullptr;
}

bool hasPointerTypeAttribute(clang::Decl *decl) {
  auto attr = getPointerTypeAttribute(decl);
  return !!attr;
}

PointerType getPointerTypeFromAttribute(clang::Decl *decl) {
  auto attr = getPointerTypeAttribute(decl);
  auto arg = attr->args().begin()[0];
  clang::IntegerLiteral *literal = llvm::dyn_cast<clang::IntegerLiteral>(arg);
  return static_cast<PointerType>(literal->getValue().getSExtValue());
}