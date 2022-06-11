#ifndef EXPRESSION_WRITER_H
#define EXPRESSION_WRITER_H
#include "Types.h"
#include "VarManager.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <sstream>
#include <string>
#include <vector>

class ExpressionWriter : public clang::RecursiveASTVisitor<ExpressionWriter> {

private:
  clang::ASTContext &context;
  VarManager &varManager;
  clang::Expr *expression;
  std::map<clang::Expr *, PointerType> &ptrTypes;

  PointerType getType(clang::Expr *expr);

  std::stringstream stream;

  std::vector<std::string> varDecls;

  std::string createVarDecl(clang::Expr *expr);
  void wrapReadCall(clang::Expr *expr);

public:
  explicit ExpressionWriter(clang::ASTContext &context, VarManager &varManager,
                            clang::Expr *expression,
                            std::map<clang::Expr *, PointerType> &ptrTypes)
      : context(context), varManager(varManager), expression(expression),
        ptrTypes(ptrTypes){};
  void run();
  std::string evaluateToString();

  bool VisitDeclRefExpr(clang::DeclRefExpr *expr);
  bool VisitUnaryOperator(clang::UnaryOperator *op);
  bool VisitBinaryOperator(clang::BinaryOperator *op);
  bool VisitIntegerLiteral(clang::IntegerLiteral *literal);
  bool VisitFloatingLiteral(clang::FloatingLiteral *literal);
  bool VisitCharacterLiteral(clang::CharacterLiteral *literal);
  bool VisitStringLiteral(clang::StringLiteral *literal);
  bool VisitCastExpr(clang::CastExpr *expr);
  bool VisitCallExpr(clang::CallExpr *expr);
  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *expr);
  bool VisitInitListExpr(clang::InitListExpr *expr);
  bool VisitDesignatedInitExpr(clang::DesignatedInitExpr *expr);
  bool VisitMemberExpr(clang::MemberExpr *expr);
  bool VisitParenExpr(clang::ParenExpr *expr);
  bool VisitUnaryExprOrTypeTraitExpr(clang::UnaryExprOrTypeTraitExpr *expr);
};

#endif