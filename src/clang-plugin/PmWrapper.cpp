#include "GlobalEvaluator.h"
#include "PointerTypeAttribute.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/raw_ostream.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Sema/Sema.h>
#include <memory>
#include <vector>

using namespace clang;

class PmWrapperASTConsumer : public clang::ASTConsumer {
public:
  explicit PmWrapperASTConsumer(ASTContext *Ctx, Rewriter &rewriter)
      : rewriter(rewriter) {}

  void HandleTranslationUnit(clang::ASTContext &Ctx) override {
    GlobalEvaluator globalEvaluator{&Ctx};
    globalEvaluator.run();
  }

private:
  Rewriter &rewriter;
};

class PmWrapperAction : public clang::PluginASTAction {
public:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &Compiler,
                    llvm::StringRef InFile) override {

    rewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return std::unique_ptr<clang::ASTConsumer>(
        std::make_unique<PmWrapperASTConsumer>(&Compiler.getASTContext(),
                                               rewriter));
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {

    return true;
  }

private:
  Rewriter rewriter;
};

static ParsedAttrInfoRegistry::Add<PointerTypeAttribute>
    Z("pointer-type-attribute", "Pointer type attribute");
static FrontendPluginRegistry::Add<PmWrapperAction>
    X("pm-wrapper-clang-plugin",
      /*Description=*/"The PmWrapper plugin");
