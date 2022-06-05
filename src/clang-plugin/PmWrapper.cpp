#include "GlobalEvaluator.h"
#include "PointerTypeAttribute.h"
#include "VarManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <memory>
#include <vector>

class PmWrapperASTConsumer : public clang::ASTConsumer {
public:
  explicit PmWrapperASTConsumer(clang::ASTContext *ctx, PluginOptions &options)
      : options(options) {}

  void HandleTranslationUnit(clang::ASTContext &ctx) override {
    VarManager varManager{ctx, options};
    GlobalEvaluator globalEvaluator{ctx, varManager};
    globalEvaluator.run();
    varManager.print();

    // ctx.getTranslationUnitDecl()->print(llvm::outs());
  }

private:
  PluginOptions &options;
};

class PmWrapperAction : public clang::PluginASTAction {
public:
  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &Compiler,
                    llvm::StringRef InFile) override {
    return std::unique_ptr<clang::ASTConsumer>(
        std::make_unique<PmWrapperASTConsumer>(&Compiler.getASTContext(),
                                               options));
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
};

static clang::ParsedAttrInfoRegistry::Add<PointerTypeAttribute>
    Z("pointer-type-attribute", "Pointer type attribute");
static clang::FrontendPluginRegistry::Add<PmWrapperAction>
    X("pm-wrapper-clang-plugin",
      /*Description=*/"The PmWrapper plugin");
