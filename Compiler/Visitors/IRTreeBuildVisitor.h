#pragma once

#include "include_list.hpp"

#include "Visitor.hpp"
#include "FrameTranslator.h"
#include "ScopeLayerTree.hpp"
#include "TemplateVisitor.hpp"

#include <IRTree/TreeWrapper/SubtreeWrapper.hpp>

#include <unordered_map>

namespace ast {

class TypeVisitor;

using IrtMapping = std::unordered_map<std::string, irt::Statement*>;

class IRTreeBuildVisitor : public TemplateVisitor<irt::SubtreeWrapper*> {
 public:
  explicit IRTreeBuildVisitor(ScopeLayerTree* scope_layer_tree);

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
  void Visit(ThisExpression* this_expression) override;
  void Visit(MethodInvocationExpression* method_invocation_expression) override;
  void Visit(ExpressionList* expression_list) override;
  void Visit(NewExpression* new_expression) override;
  void Visit(ArrayLengthExpression* array_length_expression) override;

  void Visit(Declaration* declaration) override;
  void Visit(DeclarationList* declaration_list) override;
  void Visit(VariableDeclaration* variable_declaration) override;
  void Visit(ClassDeclaration* class_declaration) override;
  void Visit(MethodDeclaration* method_declaration) override;
  void Visit(ClassDeclarationList* class_declaration_list) override;
  void Visit(Formal* formal) override;
  void Visit(FormalList* formal_list) override;

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
  void Visit(MethodInvocationStatement* method_invocation_statement) override;

  void Visit(NamedEntity* named_entity) override;
  void Visit(NamedVariable* named_variable) override;
  void Visit(NamedArrayElement* named_array_element) override;

  void Visit(Type* type) override;
  void Visit(SimpleType* simple_type) override;
  void Visit(ArrayType* array_type) override;

  ScopeLayer* GetCurrentLayer() const;
  std::string GetCurrentClassName() const;

  IrtMapping GetMethodTrees();

 private:
  ScopeLayer* current_layer_ = nullptr;
  ScopeLayerTree* scope_layer_tree_;

  // For access to ScopeLayerTree information in this visitor
  std::stack<int> layer_indices_;  // next scope index among children of parent scope

  irt::FrameTranslator* current_frame_ = nullptr;
  std::unordered_map<std::string, irt::FrameTranslator*> frame_translator_;

  TypeVisitor* type_visitor_;
  std::string current_class_name_;

  IrtMapping method_trees_;
};

} // namespace ast