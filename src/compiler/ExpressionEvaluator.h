#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H
#include "FunctionEvaluator.h"
#include "VarManager.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <map>
#include <set>
#include <stack>
#include <vector>

class ExpressionEvaluator
    : public clang::RecursiveASTVisitor<ExpressionEvaluator> {

private:
  clang::ASTContext &context;
  VarManager &varManager;
  clang::FunctionDecl *function;
  clang::Expr *expression;
  clang::SourceLocation beforeInsertLoc;

  std::map<clang::Expr *, PointerType> ptrTypes;
  PointerType currentPointerType;
  std::stack<clang::DeclRefExpr *> lastRefExpressions;

  PointerType getType(clang::Expr *expr);
  void setType(clang::Expr *expr, PointerType type);
  clang::DeclRefExpr *getLastRefExpr();

public:
  explicit ExpressionEvaluator(clang::ASTContext &context,
                               VarManager &varManager,
                               clang::FunctionDecl *function, clang::Expr *expr,
                               clang::SourceLocation beforeInsertLoc)
      : context(context), varManager(varManager), function(function),
        expression(expr), beforeInsertLoc(beforeInsertLoc){};
  PointerType run();
  bool VisitDeclRefExpr(clang::DeclRefExpr *expr);
  bool VisitUnaryOperator(clang::UnaryOperator *op);
  bool VisitBinaryOperator(clang::BinaryOperator *op);
  bool VisitUnaryExprOrTypeTraitExpr(clang::UnaryExprOrTypeTraitExpr *expr);
  bool VisitConditionalOperator(clang::ConditionalOperator *expr);
  bool VisitIntegerLiteral(clang::IntegerLiteral *literal);
  bool VisitFloatingLiteral(clang::FloatingLiteral *literal);
  bool VisitCharacterLiteral(clang::CharacterLiteral *literal);
  bool VisitStringLiteral(clang::StringLiteral *literal);
  bool VisitCastExpr(clang::CastExpr *expr);
  bool VisitCallExpr(clang::CallExpr *expr);
  bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr *expr);
  bool VisitInitListExpr(clang::InitListExpr *expr);
  bool VisitMemberExpr(clang::MemberExpr *expr);
  bool VisitParenExpr(clang::ParenExpr *expr);
};

#endif