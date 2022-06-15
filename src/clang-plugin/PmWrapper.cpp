#include "GlobalEvaluator.h"
#include "PointerTypeAttribute.h"
#include "VarManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/PreprocessorOptions.h"
#include <clang/Frontend/FrontendAction.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <memory>
#include <vector>

#include "clang/CodeGen/CodeGenAction.h"
#include "llvm/Support/MemoryBuffer.h"

class PmWrapperASTConsumer : public clang::ASTConsumer {
public:
  explicit PmWrapperASTConsumer(clang::ASTContext *ctx, PluginOptions &options,
                                clang::Rewriter &rewriter)
      : options(options), rewriter(rewriter) {}

  void HandleTranslationUnit(clang::ASTContext &ctx) override {
    VarManager varManager{ctx, options, rewriter};
    GlobalEvaluator globalEvaluator{ctx, varManager};
    globalEvaluator.run();
    varManager.print();
    varManager.printPretty();

    rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID())
        .write(llvm::outs());
  }

private:
  PluginOptions &options;
  clang::Rewriter &rewriter;
};

class PmWrapperAction : public clang::PluginASTAction {
public:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &compiler,
                    llvm::StringRef inFile) override {

    auto &sourceManager{compiler.getSourceManager()};
    auto &langOpts{compiler.getLangOpts()};

    rewriter.setSourceMgr(sourceManager, langOpts);

    return std::unique_ptr<clang::ASTConsumer>(
        std::make_unique<PmWrapperASTConsumer>(&compiler.getASTContext(),
                                               options, rewriter));
  }

  bool ParseArgs(const clang::CompilerInstance &CI,
                 const std::vector<std::string> &args) override {

    if (args.size() < 2) {
      return false;
    }

    if (args[0] == "-source-path") {
      options.sourcePath = args[1];
    }
    return true;
  }

private:
  PluginOptions options;
  clang::Rewriter rewriter;
};

static clang::ParsedAttrInfoRegistry::Add<PointerTypeAttribute>
    Z("pointer-type-attribute", "Pointer type attribute");
static clang::FrontendPluginRegistry::Add<PmWrapperAction>
    X("pm-wrapper-clang-plugin",
      /*Description=*/"The PmWrapper plugin");
