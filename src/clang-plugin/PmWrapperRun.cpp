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

void evaluatePointers(clang::tooling::ClangTool &tool,
                      llvm::Expected<clang::tooling::CommonOptionsParser> &op,
                      clang::ASTContext &context) {
  clang::Rewriter rewriter;
  auto evaluateAction = customFrontendActionFactory(rewriter);
  tool.run(evaluateAction.get());
}

void storeExpandedVersion(clang::ASTContext &context) {
  std::error_code errorCode;
  llvm::raw_fd_ostream stream("./test_abc.c", errorCode);
  clang::LangOptions options;
  clang::PrintingPolicy policy(options);
  options.CPlusPlus = false;
  policy.Bool = false;
  policy.Nullptr = true;
  context.getTranslationUnitDecl()->print(stream, policy);
  stream.flush();
  stream.close();
}

void performRewrite() {
  clang::tooling::FixedCompilationDatabase database("./", {});

  clang::tooling::ClangTool tool(database, {"./test_abc.c"});

  clang::Rewriter rewriter;
  auto writeAction = customFrontendActionFactory(rewriter);
  tool.run(writeAction.get());

  std::error_code errorCode;
  llvm::raw_fd_ostream stream("./test_abc_out.c", errorCode);
  rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(stream);
  stream.flush();
  stream.close();
}

int main(int argc, const char **argv) {

  auto op = clang::tooling::CommonOptionsParser::create(argc, argv, Category);

  clang::tooling::ClangTool tool(op->getCompilations(),
                                 op->getSourcePathList());

  std::vector<std::unique_ptr<clang::ASTUnit>> asts;
  tool.buildASTs(asts);

  auto &context = asts[0]->getASTContext();

  evaluatePointers(tool, op, context);

  storeExpandedVersion(context);

  performRewrite();

  return 0;
}