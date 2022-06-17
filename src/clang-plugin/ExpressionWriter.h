#ifndef EXPRESSION_WRITER_H
#define EXPRESSION_WRITER_H
#include "Types.h"
#include "VarManager.h"
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Type.h>
#include <sstream>
#include <string>
#include <vector>

class ExpressionWriter : public clang::RecursiveASTVisitor<ExpressionWriter> {

private:
  clang::ASTContext &context;
  VarManager &varManager;
  clang::Expr *expression;
  std::map<clang::Expr *, PointerType> &ptrTypes;
  clang::SourceLocation beforeInsertLoc;

  PointerType getType(clang::Expr *expr);

  std::stringstream stream;

  std::vector<std::string> varDecls;

  std::string createVarDecl(clang::QualType type, bool applyPointer);
  void wrapReadCall(clang::Expr *expr);

  void addWriteCall(clang::Expr *pmPtr, std::function<void()> writeOffset,
                    clang::QualType valueType,
                    std::function<void(std::string)> writeValue,
                    bool offsetCharSteps, bool returnOldValue);

  bool addAssignment(clang::Expr *lhs, clang::QualType rhsType,
                     std::function<void()> writeRhs, bool isCompoundAssignment,
                     std::string opcode, bool returnOldValue);

  uint64_t getMemberOffset(clang::MemberExpr *expr);

public:
  explicit ExpressionWriter(clang::ASTContext &context, VarManager &varManager,
                            clang::Expr *expression,
                            std::map<clang::Expr *, PointerType> &ptrTypes,
                            clang::SourceLocation beforeInsertLoc)
      : context(context), varManager(varManager), expression(expression),
        ptrTypes(ptrTypes), beforeInsertLoc(beforeInsertLoc){};
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