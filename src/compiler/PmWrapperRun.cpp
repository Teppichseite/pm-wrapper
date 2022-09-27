#include "GlobalEvaluator.h"
#include "PointerTypeAttribute.h"
#include "VarManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/PreprocessorOptions.h"
#include <clang/AST/ASTImporter.h>
#include <clang/AST/Decl.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/Utils.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <llvm-13/llvm/ADT/ArrayRef.h>
#include <llvm-13/llvm/Support/Casting.h>
#include <llvm-13/llvm/Support/CommandLine.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <memory>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "clang/CodeGen/CodeGenAction.h"
#include "llvm/Support/MemoryBuffer.h"

class PmWrapperASTConsumer : public clang::ASTConsumer {
public:
  explicit PmWrapperASTConsumer(PluginOptions &options,
                                clang::Rewriter &rewriter)
      : options(options), rewriter(rewriter) {}

  void HandleTranslationUnit(clang::ASTContext &ctx) override {
    VarManager varManager{ctx, options, rewriter};
    GlobalEvaluator globalEvaluator{ctx, varManager};
    globalEvaluator.run();
    varManager.print();
    varManager.printPretty();
  }

private:
  PluginOptions &options;
  clang::Rewriter &rewriter;
};

class PmWrapperAction : public clang::ASTFrontendAction {
public:
  PmWrapperAction(clang::Rewriter &rewriter) : rewriter(rewriter) {}

  std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &compiler,
                    llvm::StringRef inFile) override {

    auto &sourceManager{compiler.getSourceManager()};
    auto &langOpts{compiler.getLangOpts()};

    langOpts.CPlusPlus = false;

    rewriter.setSourceMgr(sourceManager, langOpts);

    return std::unique_ptr<clang::ASTConsumer>(
        std::make_unique<PmWrapperASTConsumer>(options, rewriter));
  }

private:
  PluginOptions options;
  clang::Rewriter &rewriter;
};

static llvm::cl::OptionCategory Category("");

std::unique_ptr<clang::tooling::FrontendActionFactory>
customFrontendActionFactory(clang::Rewriter &rewriter) {
  class CustomFrontendActionFactory
      : public clang::tooling::FrontendActionFactory {
  public:
    CustomFrontendActionFactory(clang::Rewriter &rewriter)
        : rewriter(rewriter) {}

    std::unique_ptr<clang::FrontendAction> create() override {
      return std::make_unique<PmWrapperAction>(rewriter);
    }

  private:
    clang::Rewriter &rewriter;
  };

  return std::make_unique<CustomFrontendActionFactory>(rewriter);
}

int compile(clang::tooling::ClangTool &tool,
            llvm::Expected<clang::tooling::CommonOptionsParser> &op,
            clang::ASTContext &context) {
  clang::Rewriter rewriter;
  auto evaluateAction = customFrontendActionFactory(rewriter);
  tool.run(evaluateAction.get());

  auto outputPath = std::getenv("PM_WRAPPER_OUTPUT_PATH");
  if (!outputPath) {
    llvm::errs() << "Please specify env variable PM_WRAPPER_OUTPUT_PATH! \n";
    return 1;
  }
  auto outputFile = std::string{outputPath} + ".c";

  std::error_code errorCode;
  llvm::raw_fd_ostream stream(outputFile, errorCode);
  rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(stream);
  stream.flush();
  stream.close();

  auto cmd = "clang -c " + outputFile + " -o " + std::string{outputPath} + ".o";
  if (system(cmd.c_str())) {
    return 1;
  };
  return 0;
}

static clang::ParsedAttrInfoRegistry::Add<PointerTypeAttribute>
    Z("pointer-type-attribute", "Pointer type attribute");

int main(int argc, const char **argv) {

  auto op = clang::tooling::CommonOptionsParser::create(argc, argv, Category);

  clang::tooling::ClangTool tool(op->getCompilations(),
                                 op->getSourcePathList());

  std::vector<std::unique_ptr<clang::ASTUnit>> asts;
  tool.buildASTs(asts);

  auto &context = asts[0]->getASTContext();

  return compile(tool, op, context);
}