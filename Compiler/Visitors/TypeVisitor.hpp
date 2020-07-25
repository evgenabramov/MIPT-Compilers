#pragma once

#include "IRTreeBuildVisitor.h"

#include <string>

namespace ast {

class TypeVisitor : public TemplateVisitor<std::string> {
 public:
  explicit TypeVisitor(IRTreeBuildVisitor* irtree_build_visitor)
      : irtree_build_visitor_(irtree_build_visitor) {}

  void Visit(Program* program) override {}
  void Visit(MainClass* main_class) override {}
  void Visit(Expression* expression) override {}

  void Visit(AddExpression* add_expression) override {
    tos_value_ = "int";
  }

  void Visit(AndExpression* and_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(BoolExpression* bool_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(DivExpression* div_expression) override {
    tos_value_ = "int";
  }

  void Visit(GreaterEqualExpression* greater_equal_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(GreaterExpression* greater_expression) override {
    tos_value_ = "int";
  }

  void Visit(IdentExpression* ident_expression) override {
    const std::string& name = ident_expression->GetIdentifier();
    auto type = irtree_build_visitor_->GetCurrentLayer()->Get(name);
    tos_value_ = type->GetTypeName();
  }

  void Visit(IntExpression* int_expression) override {
    tos_value_ = "int";
  }

  void Visit(LessEqualExpression* less_equal_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(LessExpression* less_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(MulExpression* mul_expression) override {
    tos_value_ = "int";
  }

  void Visit(NotExpression* not_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(OrExpression* or_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(SubExpression* sub_expression) override {
    tos_value_ = "int";
  }

  void Visit(EqualExpression* equal_expression) override {
    tos_value_ = "boolean";
  }

  void Visit(ModExpression* mod_expression) override {
    tos_value_ = "int";
  }

  void Visit(ArrayAccessExpression* array_access_expression) override {
    auto type = irtree_build_visitor_->GetCurrentLayer()->Get(array_access_expression->GetArrayName());
    tos_value_ = type->GetTypeName();
  }

  void Visit(NewArrayExpression* new_array_expression) override {
    tos_value_ = new_array_expression->GetSimpleType()->GetIdentifier();
  }

  void Visit(NewExpression* new_expression) override {
    tos_value_ = new_expression->GetIdentifier();
  }

  void Visit(ThisExpression* this_expression) override {
    tos_value_ = irtree_build_visitor_->GetCurrentClassName();
  }

  void Visit(MethodInvocationExpression* method_invocation_expression) override {
    std::string object_type = Accept(method_invocation_expression->GetMethodInvocation()->GetExpression());
    std::string method_name = method_invocation_expression->GetMethodInvocation()->GetIdentifier();

    auto methods = ClassStorage::GetInstance().GetClassMethods(object_type);
    if (methods.find(Symbol(method_name)) == methods.end()) {
      throw std::runtime_error("\'" + object_type + "\' method not found: " + method_name);
    }
    auto result = methods[method_name];
    tos_value_ = methods[method_name]->GetTypeName();
  }

  void Visit(ExpressionList* expression_list) override {}

  void Visit(ArrayLengthExpression* array_length_expression) override {
    tos_value_ = "int";
  }

  void Visit(Declaration* declaration) override {}
  void Visit(DeclarationList* declaration_list) override {}
  void Visit(VariableDeclaration* variable_declaration) override {}
  void Visit(ClassDeclaration* class_declaration) override {}
  void Visit(MethodDeclaration* method_declaration) override {}
  void Visit(ClassDeclarationList* class_declaration_list) override {}
  void Visit(Formal* formal) override {}
  void Visit(FormalList* formal_list) override {}

  void Visit(MethodInvocation* method_invocation) override {}

  void Visit(Statement* statement) override {}
  void Visit(StatementList* statement_list) override {}
  void Visit(AssignmentStatement* assignment_statement) override {}
  void Visit(IfStatement* if_statement) override {}
  void Visit(PrintStatement* print_statement) override {}
  void Visit(ReturnStatement* return_statement) override {}
  void Visit(VariableDeclarationStatement* variable_declaration_statement) override {}
  void Visit(WhileStatement* while_statement) override {}
  void Visit(AssertStatement* assert_statement) override {}
  void Visit(IfElseStatement* if_else_statement) override {}
  void Visit(ScopeStatement* scope_statement) override {}
  void Visit(MethodInvocationStatement* method_invocation_statement) override {}

  void Visit(NamedEntity* named_entity) override {}
  void Visit(NamedVariable* named_variable) override {}
  void Visit(NamedArrayElement* named_array_element) override {}

  void Visit(Type* type) override {}
  void Visit(SimpleType* simple_type) override {}
  void Visit(ArrayType* array_type) override {}

 private:
  IRTreeBuildVisitor* irtree_build_visitor_;
};

} // namespace ast