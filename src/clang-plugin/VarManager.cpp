#include "VarManager.h"
#include "ExpressionEvaluator.h"
#include "PointerTypeAttribute.h"
#include "Types.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclBase.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Type.h>
#include <clang/Basic/DarwinSDKInfo.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <experimental/filesystem>
#include <llvm-13/llvm/Support/JSON.h>
#include <llvm-13/llvm/Support/raw_ostream.h>
#include <random>
#include <sstream>
#include <string>

using file_path = std::experimental::filesystem::path;

std::string getRandomString() {
  std::string chars(
      "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(chars.begin(), chars.end(), generator);

  return chars.substr(0, 10);
}

VarManager::VarManager(clang::ASTContext &context, PluginOptions &options,
                       clang::Rewriter &rewriter)
    : context(context), options(options), rewriter(rewriter),
      additionalVarCount(0), additionalVarPostfix(getRandomString()) {}

PointerType VarManager::getVariableType(clang::VarDecl *decl) {
  return variables[decl];
}

FunctionType &VarManager::getFunctionType(clang::FunctionDecl *decl) {
  return functions[decl];
}

bool VarManager::isVariableSet(clang::VarDecl *decl) {
  return variables.count(decl);
}

bool VarManager::isFunctionSet(clang::FunctionDecl *decl) {
  return functions.count(decl);
}

void VarManager::setVariable(clang::VarDecl *decl, PointerType type) {
  variables[decl] = type;
}

void VarManager::setFunctionType(clang::FunctionDecl *decl, FunctionType type) {
  functions[decl] = type;
  if (decl->getNameAsString() == "pm_read_object") {
    pmWrapperReadObjectDecl = decl;
    return;
  }

  if (decl->getNameAsString() == "pm_write_object") {
    pmWrapperWriteObjectDecl = decl;
    return;
  }
}

std::string pointerTypeToString(PointerType type) {
  switch (type) {
  case PM:
    return "PM";
  case NO_PM:
    return "NO PM";
  case UNDECLARED:
    return "UNDECLARED";
  case UNKNOWN:
    return "UNKNWON";
  case NULL_PTR:
    return "NULL";
  }
  return "INVALID TYPE";
}

std::string diagnosticLevelToString(clang::DiagnosticsEngine::Level level) {
  switch (level) {
  case clang::DiagnosticsEngine::Warning:
    return "WARNGING";
  case clang::DiagnosticsEngine::Error:
    return "ERROR";
  case clang::DiagnosticsEngine::Ignored:
  case clang::DiagnosticsEngine::Note:
  case clang::DiagnosticsEngine::Remark:
  case clang::DiagnosticsEngine::Fatal:
    return "OTHER";
    break;
  }
}

void VarManager::printPretty() {
  llvm::outs() << "\nOptions: \n";
  llvm::outs() << "Source path: " << options.sourcePath << "\n\n";

  llvm::outs() << "Variables: \n";
  for (auto entry : variables) {
    auto decl = entry.first;
    auto type = entry.second;

    llvm::outs() << decl->getNameAsString() << " -> "
                 << pointerTypeToString(type) << "\n";
  }

  llvm::outs() << "\nFunctions: \n";
  for (auto e : functions) {
    auto decl = e.first;
    auto &type = e.second;

    llvm::outs() << decl->getNameAsString() << "(";
    for (auto paramType : type.parameterTypes) {
      llvm::outs() << pointerTypeToString(paramType) << ", ";
    }
    llvm::outs() << ") -> " << pointerTypeToString(type.returnType) << "\n";
  }
  llvm::outs() << "\n";
}

struct LocationFileInfo {
  file_path path;
  unsigned int line1;
  unsigned int line2;
  unsigned int column1;
  unsigned int column2;
};

LocationFileInfo getLocationFileInfo(clang::ASTContext &context,
                                     clang::SourceRange range) {
  auto &manager = context.getSourceManager();
  auto fileId = manager.getFileID(range.getBegin());
  auto filePath = std::experimental::filesystem::canonical(
      manager.getFilename(range.getBegin()).str());
  auto line1 =
      manager.getLineNumber(fileId, manager.getFileOffset(range.getBegin()));
  auto column1 =
      manager.getColumnNumber(fileId, manager.getFileOffset(range.getBegin()));
  auto line2 =
      manager.getLineNumber(fileId, manager.getFileOffset(range.getEnd()));
  auto column2 =
      manager.getColumnNumber(fileId, manager.getFileOffset(range.getEnd()));
  return {filePath, line1, line2, column1, column2};
}

void printSymbolLocation(clang::ASTContext &context, clang::SourceLocation loc,
                         PointerType type) {
  auto info = getLocationFileInfo(context, loc);
  llvm::outs() << "PM_SYMBOL_TYPE_LOC"
               << ";" << info.path << ";" << info.line1 << "," << info.column1
               << ";" << pointerTypeToString(type) << "\n";
}

void printDiagnosticLocation(clang::ASTContext &context,
                             clang::SourceRange range,
                             clang::DiagnosticsEngine::Level level,
                             std::string message) {
  auto info = getLocationFileInfo(context, range);
  llvm::outs() << "PM_DIAGNOSTIC_LOC"
               << ";" << info.path << ";" << info.line1 << "," << info.column1
               << ";" << info.line2 << "," << info.column2 << ";"
               << diagnosticLevelToString(level) << ";" << message << "\n";
}

void VarManager::print() {
  for (auto entry : variables) {
    auto decl = entry.first;
    auto type = entry.second;
    printSymbolLocation(context, decl->getBeginLoc(), type);
  }

  for (auto e : functions) {
    auto decl = e.first;
    auto &type = e.second;
    printSymbolLocation(context, decl->getBeginLoc(), type.returnType);
  }
};

bool isPathSubPath(file_path &parentPath, file_path &subPath) {
  for (auto p = parentPath.begin(), s = subPath.begin(); p != parentPath.end();
       ++p, ++s) {

    if (s == subPath.end() || *p != *s) {
      return false;
    }
  }
  return true;
}

bool VarManager::isSymbolPartOfSource(clang::Decl *decl) {
  auto &manager = context.getSourceManager();
  file_path relativeSymbolPath{manager.getFilename(decl->getLocation()).str()};
  auto absoluteSymbolPath =
      std::experimental::filesystem::canonical(relativeSymbolPath);
  auto absoluteSourcePath =
      std::experimental::filesystem::canonical(options.sourcePath);
  return isPathSubPath(absoluteSourcePath, absoluteSymbolPath);
};

void VarManager::registerVariable(clang::FunctionDecl *funcDecl,
                                  clang::VarDecl *decl) {

  if (!isSymbolPartOfSource(decl)) {
    return;
  }

  if (hasPointerTypeAttribute(decl)) {
    setVariable(decl, getPointerTypeFromAttribute(decl));
  } else {
    setVariable(decl, PointerType::UNDECLARED);
  }

  if (decl->hasInit()) {
    ExpressionEvaluator evaluator{context, *this, funcDecl, decl->getInit(),
                                  decl->getBeginLoc()};
    auto type = evaluator.run();
    if (!hasPointerTypeAttribute(decl)) {
      setVariable(decl, type);
    }
  }
}

clang::CallExpr *VarManager::CreatePmReadCallExpr(clang::Expr *arg) {

  auto declRef = clang::DeclRefExpr::Create(
      context, {}, {}, pmWrapperReadObjectDecl, false, clang::SourceLocation{},
      pmWrapperReadObjectDecl->getType(), clang::ExprValueKind::VK_LValue);

  auto callExpr = clang::CallExpr::Create(
      context, declRef, {arg}, pmWrapperReadObjectDecl->getReturnType(),
      clang::ExprValueKind::VK_LValue, {}, {});

  return callExpr;
};

clang::CallExpr *VarManager::CreatePmWriteCallExpr(clang::Expr *pmPtr,
                                                   clang::Expr *vmPtr,
                                                   clang::Expr *size) {
  auto declRef = clang::DeclRefExpr::Create(
      context, {}, {}, pmWrapperWriteObjectDecl, false, clang::SourceLocation{},
      pmWrapperWriteObjectDecl->getType(), clang::ExprValueKind::VK_LValue);

  auto callExpr =
      clang::CallExpr::Create(context, declRef, {pmPtr, vmPtr, size},
                              pmWrapperReadObjectDecl->getReturnType(),
                              clang::ExprValueKind::VK_LValue, {}, {});

  return callExpr;
};

void VarManager::reportDiagnostic(clang::DiagnosticsEngine::Level level,
                                  clang::SourceRange range,
                                  std::string message) {
  auto &de = context.getDiagnostics();
  auto &ids = de.getDiagnosticIDs();
  auto id = ids->getCustomDiagID(
      static_cast<clang::DiagnosticIDs::Level>(level), message);
  de.Report(range.getBegin(), id)
      .AddSourceRange(clang::CharSourceRange::getCharRange(range));
  printDiagnosticLocation(context, range, level, message);
}

void VarManager::reportError(clang::SourceRange range, std::string message) {
  reportDiagnostic(clang::DiagnosticsEngine::Level::Error, range, message);
}

void VarManager::reportWarning(clang::SourceRange range, std::string message) {
  reportDiagnostic(clang::DiagnosticsEngine::Level::Warning, range, message);
}

clang::Rewriter &VarManager::getRewriter() { return rewriter; }

std::string VarManager::createAdditionalVarName() {
  std::stringstream stream;
  stream << "var_";
  stream << additionalVarPostfix;
  stream << "_";
  stream << additionalVarCount;

  additionalVarCount++;

  return stream.str();
};
