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
      if (program->GetClassDeclarationList() != nullptr) {
          program->GetClassDeclarationList()->Accept(this);
      }
      --num_tabs_;
  }

  void Visit(MainClass* main_class) override {
      PrintTabs();
      out_ << "MainClass [name: " << main_class->GetName() << "]:" << std::endl;
      ++num_tabs_;
      main_class->GetStatementList()->Accept(this);
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
      out_ << "ArrayAccessExpression [array name: "
           << array_access_expression->GetArrayName() << "]" << std::endl;
      ++num_tabs_;
      array_access_expression->GetExpression()->Accept(this);
      --num_tabs_;
  }

  void Visit(NewArrayExpression* new_array_expression) override {
      PrintTabs();
      out_ << "NewArrayExpression [type name: "
           << new_array_expression->GetSimpleType()->GetIdentifier() << "]" << std::endl;
      ++num_tabs_;
      new_array_expression->GetExpression()->Accept(this);
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
      if (declaration_list->GetTail() != nullptr) {
          declaration_list->GetTail()->Accept(this);
      }
      --num_tabs_;
  }

  void Visit(VariableDeclaration* variable_declaration) override {
      PrintTabs();
      out_ << "VariableDeclaration [variable name: " << variable_declaration->GetVariableName() << "]" << std::endl;
      ++num_tabs_;
      variable_declaration->GetType()->Accept(this);
      --num_tabs_;
  }

  void Visit(ClassDeclaration* class_declaration) override {
      PrintTabs();
      out_ << "ClassDeclaration [class name: " << class_declaration->GetClassName() << "]" << std::endl;
      ++num_tabs_;
      class_declaration->GetDeclarationList()->Accept(this);
      --num_tabs_;
  }

  void Visit(MethodDeclaration* method_declaration) override {
      PrintTabs();
      out_ << "MethodDeclaration [method type: " << method_declaration->GetType()->GetIdentifier()
           << " method name: " << method_declaration->GetIdentifier() << "]" << std::endl;
      ++num_tabs_;
      method_declaration->GetFormalList()->Accept(this);
      method_declaration->GetStatementList()->Accept(this);
      --num_tabs_;
  }

  void Visit(ClassDeclarationList* class_declaration_list) override {
      if (class_declaration_list == nullptr) {
          return;
      }
      PrintTabs();
      out_ << "ClassDeclarationList: " << std::endl;
      ++num_tabs_;
      class_declaration_list->GetFirstItem()->Accept(this);
      if (class_declaration_list->GetTail() != nullptr) {
          class_declaration_list->GetTail()->Accept(this);
      }
      --num_tabs_;
  }

  // Terminal position
  void Visit(Formal* formal) override {
      PrintTabs();
      out_ << "Formal: [type: " << formal->GetType()->GetIdentifier() << " name: "
           << formal->GetIdentifier() << "]" << std::endl;
  }

  void Visit(FormalList* formal_list) override {
      if (formal_list == nullptr) {
          return;
      }
      PrintTabs();
      out_ << "FormalList: " << std::endl;
      ++num_tabs_;
      formal_list->GetFirstItem()->Accept(this);
      if (formal_list->GetTail() != nullptr) {
          formal_list->GetTail()->Accept(this);
      }
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
      if (statement_list->GetTail() != nullptr) {
          statement_list->GetTail()->Accept(this);
      }
      --num_tabs_;
  }

  void Visit(AssignmentStatement* assignment_statement) override {
      PrintTabs();
      out_ << "AssignmentStatement: " << std::endl;
      ++num_tabs_;
      assignment_statement->GetNamedEntity()->Accept(this);
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

  void Visit(ScopeStatement* scope_statement) override {
      PrintTabs();
      out_ << "ScopeStatement: " << std::endl;
      ++num_tabs_;
      scope_statement->GetStatementList()->Accept(this);
      --num_tabs_;
  }

  // Base class (terminal position)
  void Visit(NamedEntity* named_entity) override {
      PrintTabs();
      out_ << "NamedEntity [name: " << named_entity->GetName() << "]" << std::endl;
  }

  // Terminal position
  void Visit(NamedVariable* named_variable) override {
      PrintTabs();
      out_ << "NamedVariable [name: " << named_variable->GetName() << "]" << std::endl;
  }

  // Terminal position
  void Visit(NamedArrayElement* named_array) override {
      PrintTabs();
      out_ << "NamedArrayElement [name: " << named_array->GetName() << "]" << std::endl;
      ++num_tabs_;
      named_array->GetIndexExpression()->Accept(this);
      --num_tabs_;
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