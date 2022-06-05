#ifndef UTILS_H
#define UTILS_H
#include <clang/AST/ASTContext.h>
#include <clang/Basic/SourceLocation.h>

template <unsigned int N>
void reportDiagnostic(clang::ASTContext &context,
                      clang::DiagnosticsEngine::Level level,
                      clang::SourceLocation location,
                      const char (&message)[N]) {
  auto &de = context.getDiagnostics();
  const auto id = de.getCustomDiagID(level, message);

  de.Report(location, id);
}

template <unsigned int N>
void reportError(clang::ASTContext &context, clang::SourceLocation location,
                 const char (&message)[N]) {
  reportDiagnostic(context, clang::DiagnosticsEngine::Level::Error, location,
                   message);
}

template <unsigned int N>
void reportWarning(clang::ASTContext &context, clang::SourceLocation location,
                   const char (&message)[N]) {
  reportDiagnostic(context, clang::DiagnosticsEngine::Level::Warning, location,
                   message);
}

#endif