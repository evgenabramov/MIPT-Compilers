#pragma once

#include "include_list.hpp"

#include <fstream>
#include <unordered_map>
#include <memory>
#include <exception>

namespace ast {

class Interpreter : public Visitor {
 public:
  explicit Interpreter(const std::string& filename)
      : out_(filename),
        tos_value_(std::make_shared<IntValue>(0)),
        is_returned_(false),
        return_value_(std::make_shared<IntValue>(0)) {}

  ~Interpreter() {
      out_.close();
  }

  void PrintReturnValue() {
      if (!out_) {
          throw std::runtime_error("File not open");
      }

      out_ << "Result type: " << return_value_->GetTypeIdentifier() << std::endl;

      out_ << "Value: ";
      if (return_value_->GetTypeIdentifier() == "int") {
          out_ << return_value_->toInt() << std::endl;
      } else if (return_value_->GetTypeIdentifier() == "boolean") {
          out_ << return_value_->toBool() << std::endl;
      } else {
          throw std::runtime_error("Return type not recognised");
      }
  }

  void Visit(Program* program) override {
      program->GetMainClass()->Accept(this);
  }

  void Visit(MainClass* main_class) override {
      main_class->GetStatements()->Accept(this);
  }

  // Base class
  void Visit(Expression* expression) override {}

  void Visit(AddExpression* add_expression) override {
      add_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      add_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<IntValue>(lhs + rhs);
  }

  void Visit(AndExpression* and_expression) override {
      and_expression->GetLeftExpression()->Accept(this);
      bool lhs = tos_value_->toBool();
      and_expression->GetRightExpression()->Accept(this);
      bool rhs = tos_value_->toBool();
      tos_value_ = std::make_shared<BoolValue>(lhs && rhs);
  }

  // Terminal position
  void Visit(BoolExpression* bool_expression) override {
      tos_value_ = std::make_shared<BoolValue>(bool_expression->GetValue());
  }

  void Visit(DivExpression* div_expression) override {
      div_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      div_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<IntValue>(lhs / rhs);
  }

  void Visit(GreaterEqualExpression* greater_equal_expression) override {
      greater_equal_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      greater_equal_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<BoolValue>(lhs >= rhs);
  }

  void Visit(GreaterExpression* greater_expression) override {
      greater_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      greater_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<BoolValue>(lhs > rhs);
  }

  // Terminal position
  void Visit(IdentExpression* ident_expression) override {
      tos_value_ = variables_[ident_expression->GetIdentifier()];
  }

  // Terminal position
  void Visit(IntExpression* int_expression) override {
      tos_value_ = std::make_shared<IntValue>(int_expression->GetValue());
  }

  void Visit(LessEqualExpression* less_equal_expression) override {
      less_equal_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      less_equal_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<BoolValue>(lhs <= rhs);
  }

  void Visit(LessExpression* less_expression) override {
      less_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      less_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<BoolValue>(lhs < rhs);
  }

  void Visit(MulExpression* mul_expression) override {
      mul_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      mul_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<IntValue>(lhs * rhs);
  }

  void Visit(NotExpression* not_expression) override {
      not_expression->GetExpression()->Accept(this);
      bool value = tos_value_->toBool();
      tos_value_ = std::make_shared<BoolValue>(!value);
  }

  void Visit(OrExpression* or_expression) override {
      or_expression->GetLeftExpression()->Accept(this);
      bool lhs = tos_value_->toBool();
      or_expression->GetRightExpression()->Accept(this);
      bool rhs = tos_value_->toBool();
      tos_value_ = std::make_shared<BoolValue>(lhs || rhs);
  }

  void Visit(SubExpression* sub_expression) override {
      sub_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      sub_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<IntValue>(lhs - rhs);
  }

  void Visit(EqualExpression* equal_expression) override {
      equal_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      equal_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<BoolValue>(lhs == rhs);
  }

  void Visit(ModExpression* mod_expression) override {
      mod_expression->GetLeftExpression()->Accept(this);
      int lhs = tos_value_->toInt();
      mod_expression->GetRightExpression()->Accept(this);
      int rhs = tos_value_->toInt();
      tos_value_ = std::make_shared<IntValue>(lhs % rhs);
  }

  // Base class
  void Visit(Declaration* declaration) override {}

  void Visit(DeclarationList* declaration_list) override {
      if (declaration_list->GetFirstItem() == nullptr) {
          return;
      }
      declaration_list->GetFirstItem()->Accept(this);
      declaration_list->GetTail()->Accept(this);
  }

  // Terminal position
  void Visit(VariableDeclaration* variable_declaration) override {
      const std::string& variable_name = variable_declaration->GetVariableName();
      std::string type_name = variable_declaration->GetType()->GetIdentifier();
      if (type_name == "int") {
          variables_[variable_name] = std::make_shared<IntValue>();
      } else if (type_name == "boolean") {
          variables_[variable_name] = std::make_shared<BoolValue>();
      } else {
          throw std::logic_error("Bad variable type");
      }
  }

  // Base class
  void Visit(Statement* statement) override {}

  void Visit(StatementList* statement_list) override {
      if (statement_list->GetHead() == nullptr) {
          return;
      }
      statement_list->GetHead()->Accept(this);
      statement_list->GetTail()->Accept(this);
  }

  void Visit(AssignmentStatement* assignment_statement) override {
      std::string variable_name = assignment_statement->GetNamedEntity()->GetName();
      if (variables_.find(variable_name) == variables_.end()) {
          throw std::logic_error("Variable is not declared");
      }
      assignment_statement->GetExpression()->Accept(this);
      if (variables_[variable_name]->GetTypeIdentifier() == "int") {
          variables_[variable_name] = std::make_shared<IntValue>(tos_value_->toInt());
      } else if (variables_[variable_name]->GetTypeIdentifier() == "boolean") {
          variables_[variable_name] = std::make_shared<BoolValue>(tos_value_->toBool());
      }
  }

  void Visit(IfStatement* if_statement) override {
      if_statement->GetExpression()->Accept(this);
      bool expression_result = tos_value_->toBool();
      if (expression_result) {
          if_statement->GetStatement()->Accept(this);
      }
  }

  void Visit(PrintStatement* print_statement) override {
      print_statement->GetExpression()->Accept(this);
      int expression_result = tos_value_->toInt();
      out_ << expression_result << std::endl;
  }

  void Visit(ReturnStatement* return_statement) override {
      return_statement->GetExpression()->Accept(this);
      return_value_ = tos_value_;
      is_returned_ = true;
  }

  void Visit(VariableDeclarationStatement* variable_declaration_statement) override {
      variable_declaration_statement->GetVariableDeclaration()->Accept(this);
  }

  void Visit(WhileStatement* while_statement) override {
      while_statement->GetExpression()->Accept(this);
      bool expression_result = tos_value_->toBool();
      while (expression_result) {
          while_statement->GetStatement()->Accept(this);

          while_statement->GetExpression()->Accept(this);
          expression_result = tos_value_->toBool();
      }
  }

  void Visit(AssertStatement* assert_statement) override {
      assert_statement->GetExpression()->Accept(this);
      bool expression_result = tos_value_->toBool();
      if (!expression_result) {
          throw std::logic_error("Assert failure");
      }
  }

  void Visit(IfElseStatement* if_else_statement) override {
      if_else_statement->GetExpression()->Accept(this);
      bool expression_result = tos_value_->toBool();
      if (expression_result) {
          if_else_statement->GetFirstStatement()->Accept(this);
      } else {
          if_else_statement->GetSecondStatement()->Accept(this);
      }
  }

  // Base class
  void Visit(NamedEntity* named_entity) override {}

  // Terminal position
  void Visit(NamedVariable* named_variable) override {}

  // Base class
  void Visit(Type* type) override {}

  void Visit(SimpleType* simple_type) override {}

  void Visit(ArrayType* array_type) override {}

 private:
  std::unordered_map<std::string, std::shared_ptr<Value>> variables_;

  std::shared_ptr<Value> tos_value_;
  std::shared_ptr<Value> return_value_;
  bool is_returned_;
  std::ofstream out_;
};

} // namespace ast