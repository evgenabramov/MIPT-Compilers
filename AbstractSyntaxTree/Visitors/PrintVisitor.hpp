#pragma once

#include "include_list.hpp"

#include <fstream>

namespace ast {

class PrintVisitor : public Visitor {
 public:
  explicit PrintVisitor(const std::string& filename) : out_(filename), num_tabs_(0) {}

  ~PrintVisitor() {
      out_.close();
  }

  void Visit(Program* program) override {
      PrintTabs();
      out_ << "Program:" << std::endl;
      ++num_tabs_;
      program->GetMainClass()->Accept(this);
      --num_tabs_;
  }

  void Visit(MainClass* main_class) override {
      PrintTabs();
      out_ << "MainClass [name: " << main_class->GetName() << "]:" << std::endl;
      ++num_tabs_;
      main_class->GetStatements()->Accept(this);
      --num_tabs_;
  }

  // Base class
  void Visit(Expression* expression) override {}

  void Visit(AddExpression* add_expression) override {
      PrintTabs();
      out_ << "AddExpression:" << std::endl;
      ++num_tabs_;
      add_expression->GetLeftExpression()->Accept(this);
      add_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(AndExpression* and_expression) override {
      PrintTabs();
      out_ << "AndExpression:" << std::endl;
      ++num_tabs_;
      and_expression->GetLeftExpression()->Accept(this);
      and_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  // Terminal position
  void Visit(BoolExpression* bool_expression) override {
      PrintTabs();
      out_ << "BoolExpression [value: " << bool_expression->GetValue() << "]" << std::endl;
  }

  void Visit(DivExpression* div_expression) override {
      PrintTabs();
      out_ << "DivExpression:" << std::endl;
      ++num_tabs_;
      div_expression->GetLeftExpression()->Accept(this);
      div_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(GreaterEqualExpression* greater_equal_expression) override {
      PrintTabs();
      out_ << "GreaterEqualExpression:" << std::endl;
      ++num_tabs_;
      greater_equal_expression->GetLeftExpression()->Accept(this);
      greater_equal_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(GreaterExpression* greater_expression) override {
      PrintTabs();
      out_ << "GreaterExpression:" << std::endl;
      ++num_tabs_;
      greater_expression->GetLeftExpression()->Accept(this);
      greater_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  // Terminal position
  void Visit(IdentExpression* ident_expression) override {
      PrintTabs();
      out_ << "IdentExpression [identifier: " << ident_expression->GetIdentifier() << "]" << std::endl;
  }

  // Terminal position
  void Visit(IntExpression* int_expression) override {
      PrintTabs();
      out_ << "IntExpression [value: " << int_expression->GetValue() << "]" << std::endl;
  }

  void Visit(LessEqualExpression* less_equal_expression) override {
      PrintTabs();
      out_ << "LessEqualExpression:" << std::endl;
      ++num_tabs_;
      less_equal_expression->GetLeftExpression()->Accept(this);
      less_equal_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(LessExpression* less_expression) override {
      PrintTabs();
      out_ << "LessExpression:" << std::endl;
      ++num_tabs_;
      less_expression->GetLeftExpression()->Accept(this);
      less_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(MulExpression* mul_expression) override {
      PrintTabs();
      out_ << "MulExpression:" << std::endl;
      ++num_tabs_;
      mul_expression->GetLeftExpression()->Accept(this);
      mul_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(NotExpression* not_expression) override {
      PrintTabs();
      out_ << "NotExpression:" << std::endl;
      ++num_tabs_;
      not_expression->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(OrExpression* or_expression) override {
      PrintTabs();
      out_ << "OrExpression:" << std::endl;
      ++num_tabs_;
      or_expression->GetLeftExpression()->Accept(this);
      or_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(SubExpression* sub_expression) override {
      PrintTabs();
      out_ << "SubExpression:" << std::endl;
      ++num_tabs_;
      sub_expression->GetLeftExpression()->Accept(this);
      sub_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(EqualExpression* equal_expression) override {
      PrintTabs();
      out_ << "EqualExpression:" << std::endl;
      ++num_tabs_;
      equal_expression->GetLeftExpression()->Accept(this);
      equal_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(ModExpression* mod_expression) override {
      PrintTabs();
      out_ << "ModExpression:" << std::endl;
      ++num_tabs_;
      mod_expression->GetLeftExpression()->Accept(this);
      mod_expression->GetRightExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(ArrayAccessExpression* array_access_expression) override {
      PrintTabs();
      out_ << "ArrayAccessExpression [array name: " << array_access_expression->GetArrayName() << "]" << std::endl;
      ++num_tabs_;
      array_access_expression->GetExpression()->Accept(this);
      --num_tabs_;
  }

  // Base class
  void Visit(Declaration* declaration) override {}

  void Visit(DeclarationList* declaration_list) override {
      if (declaration_list->GetFirstItem() == nullptr) {
          return;
      }
      PrintTabs();
      out_ << "DeclarationList:" << std::endl;
      ++num_tabs_;
      declaration_list->GetFirstItem()->Accept(this);
      declaration_list->GetTail()->Accept(this);
      --num_tabs_;
  }

  void Visit(VariableDeclaration* variable_declaration) override {
      PrintTabs();
      out_ << "VariableDeclaration [variable name: " << variable_declaration->GetVariableName() << "]" << std::endl;
      ++num_tabs_;
      variable_declaration->GetSimpleType()->Accept(this);
      --num_tabs_;
  }

  void Visit(ArrayDeclaration* array_declaration) override {
      PrintTabs();
      out_ << "ArrayDeclaration [array name: " << array_declaration->GetArrayName()
           << "]:" << std::endl;
      ++num_tabs_;
      array_declaration->GetArrayType()->Accept(this);
      --num_tabs_;
  }

  // Base class
  void Visit(Statement* statement) override {}

  void Visit(StatementList* statement_list) override {
      if (statement_list->GetHead() == nullptr) {
          return;
      }
      PrintTabs();
      out_ << "StatementList: " << std::endl;
      ++num_tabs_;
      statement_list->GetHead()->Accept(this);
      statement_list->GetTail()->Accept(this);
      --num_tabs_;
  }

  void Visit(AssignmentStatement* assignment_statement) override {
      PrintTabs();
      out_ << "AssignmentStatement: " << std::endl;
      ++num_tabs_;
      assignment_statement->GetNamedVariable()->Accept(this);
      assignment_statement->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(IfStatement* if_statement) override {
      PrintTabs();
      out_ << "IfStatement: " << std::endl;
      ++num_tabs_;
      if_statement->GetExpression()->Accept(this);
      if_statement->GetStatement()->Accept(this);
      --num_tabs_;
  }

  void Visit(PrintStatement* print_statement) override {
      PrintTabs();
      out_ << "PrintStatement: " << std::endl;
      ++num_tabs_;
      print_statement->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(ReturnStatement* return_statement) override {
      PrintTabs();
      out_ << "Return statement: " << std::endl;
      ++num_tabs_;
      return_statement->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(VariableDeclarationStatement* variable_declaration_statement) override {
      PrintTabs();
      out_ << "VariableDeclarationStatement: " << std::endl;
      ++num_tabs_;
      variable_declaration_statement->GetVariableDeclaration()->Accept(this);
      --num_tabs_;
  }

  void Visit(WhileStatement* while_statement) override {
      PrintTabs();
      out_ << "WhileStatement:" << std::endl;
      ++num_tabs_;
      while_statement->GetExpression()->Accept(this);
      while_statement->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(AssertStatement* assert_statement) override {
      PrintTabs();
      out_ << "AssertStatement:" << std::endl;
      ++num_tabs_;
      assert_statement->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(IfElseStatement* if_else_statement) override {
      PrintTabs();
      out_ << "IfElseStatement: " << std::endl;
      ++num_tabs_;
      if_else_statement->GetExpression()->Accept(this);
      if_else_statement->GetFirstStatement()->Accept(this);
      if_else_statement->GetSecondStatement()->Accept(this);
      --num_tabs_;
  }

  void Visit(ArrayDeclarationStatement* array_declaration_statement) override {
      PrintTabs();
      out_ << "ArrayDeclarationStatement:" << std::endl;
      ++num_tabs_;
      array_declaration_statement->GetArrayDeclaration()->Accept(this);
      --num_tabs_;
  }

  void Visit(ArrayAssignmentStatement* array_assignment_statement) override {
      PrintTabs();
      out_ << "ArrayAssignmentStatement [array name: "
           << array_assignment_statement->GetNamedArray()->GetName() << "]:" << std::endl;
      ++num_tabs_;
      array_assignment_statement->GetSimpleType()->Accept(this);
      array_assignment_statement->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(ArrayElementAssignmentStatement* array_element_assignment_statement) override {
      PrintTabs();
      out_ << "ArrayElementAssignmentStatement [array name: "
           << array_element_assignment_statement->GetArrayName() << "]:" << std::endl;
      ++num_tabs_;
      array_element_assignment_statement->GetIndexExpression()->Accept(this);
      array_element_assignment_statement->GetValueExpression()->Accept(this);
      --num_tabs_;
  }

  // Base class
  void Visit(NamedEntity* named_entity) override {}

  // Terminal position
  void Visit(NamedVariable* named_variable) override {
      PrintTabs();
      out_ << "NamedVariable [name: " << named_variable->GetName() << "]" << std::endl;
  }

  // Terminal position
  void Visit(NamedArray* named_array) override {
      PrintTabs();
      out_ << "NamedArray [name: " << named_array->GetName() << "]" << std::endl;
  }

  // Base class
  void Visit(Type* type) override {}

  // Terminal position
  void Visit(SimpleType* simple_type) override {
      PrintTabs();
      out_ << "SimpleType [identifier: " << simple_type->GetIdentifier() << "]" << std::endl;
  }

  // Terminal position
  void Visit(ArrayType* array_type) override {
      PrintTabs();
      out_ << "ArrayType [identifier: " << array_type->GetIdentifier() << "]" << std::endl;
  }

 private:

  void PrintTabs() {
      for (size_t i = 0; i < num_tabs_; ++i) {
          out_ << "  ";
      }
  }

  std::ofstream out_;
  size_t num_tabs_;
};

} // namespace ast