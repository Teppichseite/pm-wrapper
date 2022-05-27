#ifndef POINTER_TYPE_ATTRIBUTE_H
#define POINTER_TYPE_ATTRIBUTE_H
#include "Types.h"
#include "clang/Sema/ParsedAttr.h"
#include "clang/Sema/Sema.h"
#include <clang/AST/DeclBase.h>

class PointerTypeAttribute : public clang::ParsedAttrInfo {
public:
  PointerTypeAttribute() {

    NumArgs = 1;

    static constexpr Spelling S[] = {
        {clang::ParsedAttr::AS_GNU, "pointer_type"}};
    Spellings = S;
  }
  AttrHandling
  handleDeclAttribute(clang::Sema &s, clang::Decl *decl,
                      const clang::ParsedAttr &attr) const override {

    // TODO: Check that arg is a integer literal
    std::vector<clang::Expr *> args = {attr.getArgAsExpr(0)};
    decl->addAttr(clang::AnnotateAttr::Create(
        s.Context, "pointer_type", args.data(), args.size(), attr.getRange()));
    return AttributeApplied;
  }
};

bool hasPointerTypeAttribute(clang::Decl *decl);

PointerType getPointerTypeFromAttribute(clang::Decl *decl);

#endif