#pragma once

#include "include_list.hpp"
#include "TemplateVisitor.hpp"

#include <memory>
#include <optional>

namespace ast {

class MethodCallVisitor : public TemplateVisitor<Object*> {
 public:
  MethodCallVisitor(ScopeLayer* method_scope, std::shared_ptr<MethodType> method, SimpleObject* this_object);

  void SetTree(ScopeLayerTree* tree);

  void SetParams(const std::vector<Object*>& params);

  Frame& GetFrame();

  void Visit(Program* program) override;
  void Visit(MainClass* main_class) override;
  void Visit(Expression* expression) override;
  void Visit(AddExpression* add_expression) override;
  void Visit(AndExpression* and_expression) override;
  void Visit(BoolExpression* bool_expression) override;
  void Visit(DivExpression* div_expression) override;
  void Visit(GreaterEqualExpression* greater_equal_expression) override;
  void Visit(GreaterExpression* greater_expression) override;
  void Visit(IdentExpression* ident_expression) override;
  void Visit(IntExpression* int_expression) override;
  void Visit(LessEqualExpression* less_equal_expression) override;
  void Visit(LessExpression* less_expression) override;
  void Visit(MulExpression* mul_expression) override;
  void Visit(NotExpression* not_expression) override;
  void Visit(OrExpression* or_expression) override;
  void Visit(ModExpression* mod_expression) override;
  void Visit(ArrayAccessExpression* array_access_expression) override;
  void Visit(NewArrayExpression* new_array_expression) override;
  void Visit(SubExpression* sub_expression) override;
  void Visit(EqualExpression* equal_expression) override;

  // NEW !!!
  void Visit(ThisExpression* this_expression) override;
  void Visit(MethodInvocationExpression* method_invocation_expression) override;
  void Visit(ExpressionList* expression_list) override;
  void Visit(NewExpression* new_expression) override;

  void Visit(Declaration* declaration) override;
  void Visit(DeclarationList* declaration_list) override;
  void Visit(VariableDeclaration* variable_declaration) override;
  void Visit(ClassDeclaration* class_declaration) override;
  void Visit(MethodDeclaration* method_declaration) override;
  void Visit(ClassDeclarationList* class_declaration_list) override;
  void Visit(Formal* formal) override;
  void Visit(FormalList* formal_list) override;

  // NEW !!!
  void Visit(MethodInvocation* method_invocation) override;

  void Visit(Statement* statement) override;
  void Visit(StatementList* statement_list) override;
  void Visit(AssignmentStatement* assignment_statement) override;
  void Visit(IfStatement* if_statement) override;
  void Visit(PrintStatement* print_statement) override;
  void Visit(ReturnStatement* return_statement) override;
  void Visit(VariableDeclarationStatement* variable_declaration_statement) override;
  void Visit(WhileStatement* while_statement) override;
  void Visit(AssertStatement* assert_statement) override;
  void Visit(IfElseStatement* if_else_statement) override;
  void Visit(ScopeStatement* scope_statement) override;

  // NEW !!!
  void Visit(MethodInvocationStatement* method_invocation_statement) override;

  void Visit(NamedEntity* named_entity) override;
  void Visit(NamedVariable* named_variable) override;
  void Visit(NamedArrayElement* named_array_element) override;

  void Visit(Type* type) override;
  void Visit(SimpleType* simple_type) override;
  void Visit(ArrayType* array_type) override;

 private:
  std::pair<int, int> VisitBinaryExpression(Expression* left_expression, Expression* right_expression);

  ScopeLayerTree* scope_layer_tree_;
  ScopeLayer* root_layer_;
  ScopeLayer* current_layer_; // current scope layer

  // For access to ScopeLayerTree information in this visitor
  std::stack<int> layer_indices_;  // next scope index among children of parent scope
  // TODO: Add support for if/if-else/while without brackets (no scope offset at all)

  Frame frame_; // function frame
  FunctionTable function_table_;

  SimpleObject* this_object_;

  bool is_returned_ = false; // TODO: check if is redundant
};

} // namespace ast