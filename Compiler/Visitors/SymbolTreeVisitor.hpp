#pragma once

#include "include_list.hpp"

#include <fstream>
#include <unordered_set>
#include <iostream>

namespace ast {

class SymbolTreeVisitor : public Visitor {
 public:
  explicit SymbolTreeVisitor(const std::string& filename) : out_(filename) {
      current_layer_ = new ScopeLayer();
      scope_layer_tree_ = new ScopeLayerTree(current_layer_);
  }

  ~SymbolTreeVisitor() {
      delete current_layer_;
      delete scope_layer_tree_;
      out_.close();
  }

  void Visit(Program* program) override {
      program->GetMainClass()->Accept(this);
      if (program->GetClassDeclarationList() != nullptr) {
          program->GetClassDeclarationList()->Accept(this);
      }
  }

  void Visit(MainClass* main_class) override {
      // tos_value logic for scopes
      current_label_ = Symbol("main");

      auto main_method = current_layer_->DeclareMethod(
          current_label_,
          new MethodDeclaration(
              new SimpleType("void"),
              "main",
              nullptr,
              main_class->GetStatementList())
      );

      current_layer_ = new ScopeLayer(current_layer_);
      scope_layer_tree_->LabelScope("main", current_layer_);
      is_inside_method_ = true;
      main_class->GetStatementList()->Accept(this);
      current_layer_ = current_layer_->GetParent();
      is_inside_method_ = false;
  }

  void Visit(Expression* expression) override {}

  void Visit(AddExpression* add_expression) override {}

  void Visit(AndExpression* and_expression) override {}

  void Visit(BoolExpression* bool_expression) override {}

  void Visit(DivExpression* div_expression) override {}

  void Visit(GreaterEqualExpression* greater_equal_expression) override {}

  void Visit(GreaterExpression* greater_expression) override {}

  void Visit(IdentExpression* ident_expression) override {}

  void Visit(IntExpression* int_expression) override {}

  void Visit(LessEqualExpression* less_equal_expression) override {}

  void Visit(LessExpression* less_expression) override {}

  void Visit(MulExpression* mul_expression) override {}

  void Visit(NotExpression* not_expression) override {}

  void Visit(OrExpression* or_expression) override {}

  void Visit(SubExpression* sub_expression) override {}

  void Visit(EqualExpression* equal_expression) override {}

  void Visit(ModExpression* mod_expression) override {}

  void Visit(ArrayAccessExpression* array_access_expression) override {}

  void Visit(NewArrayExpression* new_array_expression) override {}

  void Visit(Declaration* declaration) override {}

  void Visit(DeclarationList* declaration_list) override {
      if (declaration_list->GetFirstItem() == nullptr) {
          return;
      }
      declaration_list->GetFirstItem()->Accept(this);
      if (declaration_list->GetTail() != nullptr) {
          declaration_list->GetTail()->Accept(this);
      }
  }

  void Visit(VariableDeclaration* variable_declaration) override {
      if (variable_declaration->GetType()->IsSimpleType()) {
          auto primitive_simple_type = current_layer_->DeclareSimpleVariable(
              variable_declaration->GetVariableName(),
              dynamic_cast<SimpleType*>(variable_declaration->GetType())
          );

          // If object does not exist, it will be created
          if (!is_inside_method_) {
              class_types_[current_label_]->AddField(variable_declaration->GetVariableName(), primitive_simple_type);
          }
      } else {
          auto primitive_array_type = current_layer_->DeclareArrayVariable(
              variable_declaration->GetVariableName(),
              dynamic_cast<ArrayType*>(variable_declaration->GetType())
          );

          if (!is_inside_method_) {
              class_types_[current_label_]->AddField(variable_declaration->GetVariableName(), primitive_array_type);
          }
      }
  }

  void Visit(ClassDeclaration* class_declaration) override {
      current_label_ = class_declaration->GetClassName();
      class_types_[current_label_] = current_layer_->DeclareClass(current_label_, class_declaration);
      scope_layer_tree_->LabelScope(current_label_, current_layer_);
      class_declaration->GetDeclarationList()->Accept(this);
  }

  void Visit(MethodDeclaration* method_declaration) override {
      auto method_info = current_layer_->DeclareMethod(method_declaration->GetIdentifier(), method_declaration);
      class_types_[current_label_]->AddMethod(method_declaration->GetIdentifier(), method_info);
      scope_layer_tree_->LabelScope(method_declaration->GetIdentifier(), current_layer_);

      current_layer_ = new ScopeLayer(current_layer_);
      is_inside_method_ = true;
      method_declaration->GetFormalList()->Accept(this);
      method_declaration->GetStatementList()->Accept(this);
      is_inside_method_ = false;
      current_layer_ = current_layer_->GetParent();
  }

  void Visit(ClassDeclarationList* class_declaration_list) override {
      class_declaration_list->GetFirstItem()->Accept(this);
      if (class_declaration_list->GetTail() != nullptr) {
          class_declaration_list->GetTail()->Accept(this);
      }
  }

  void Visit(Formal* formal) override {
      // can only be simple type and not a field
      auto primitive_simple_type = current_layer_->DeclareSimpleVariable(
          formal->GetIdentifier(),
          dynamic_cast<SimpleType*>(formal->GetType())
      );
  }

  void Visit(FormalList* formal_list) override {
      if (formal_list == nullptr) {
          return;
      }
      formal_list->GetFirstItem()->Accept(this);
      if (formal_list->GetTail() != nullptr) {
          formal_list->GetTail()->Accept(this);
      }
  }

  void Visit(Statement* statement) override {}

  void Visit(StatementList* statement_list) override {
      if (statement_list->GetHead() == nullptr) {
          return;
      }
      statement_list->GetHead()->Accept(this);
      if (statement_list->GetTail() != nullptr) {
          statement_list->GetTail()->Accept(this);
      }
  }

  void Visit(AssignmentStatement* assignment_statement) override {}

  void Visit(IfStatement* if_statement) override {
      current_layer_ = new ScopeLayer(current_layer_);
      if_statement->GetStatement()->Accept(this);
      current_layer_ = current_layer_->GetParent();
  }

  void Visit(PrintStatement* print_statement) override {}

  void Visit(ReturnStatement* return_statement) override {}

  void Visit(VariableDeclarationStatement* variable_declaration_statement) override {
      variable_declaration_statement->GetVariableDeclaration()->Accept(this);
  }

  void Visit(WhileStatement* while_statement) override {
      current_layer_ = new ScopeLayer(current_layer_);
      while_statement->GetExpression()->Accept(this);
      current_layer_ = current_layer_->GetParent();
  }

  void Visit(AssertStatement* assert_statement) override {}

  void Visit(IfElseStatement* if_else_statement) override {
      current_layer_ = new ScopeLayer(current_layer_);
      if_else_statement->GetFirstStatement()->Accept(this);
      current_layer_ = current_layer_->GetParent();

      current_layer_ = new ScopeLayer(current_layer_);
      if_else_statement->GetSecondStatement()->Accept(this);
      current_layer_ = current_layer_->GetParent();
  }

  void Visit(ScopeStatement* scope_statement) override {
      current_layer_ = new ScopeLayer(current_layer_);
      scope_statement->GetStatementList()->Accept(this);
      current_layer_ = current_layer_->GetParent();
  }

  void Visit(NamedEntity* named_entity) override {}

  void Visit(NamedVariable* named_variable) override {}

  void Visit(NamedArrayElement* named_array_element) override {}

  // Declaration info set one level before
  void Visit(Type* type) override {}

  void Visit(SimpleType* simple_type) override {}

  void Visit(ArrayType* array_type) override {}

  const std::unordered_map<Symbol, std::shared_ptr<ClassType>>& GetClassTypes() const {
      return class_types_;
  }

  void OutputTreeLayers() {
      scope_layer_tree_->GetRoot()->PrintLayer(out_, 0);
  }

  ScopeLayer* GetRoot() const {
      return scope_layer_tree_->GetRoot();
  }

 private:
  ScopeLayerTree* scope_layer_tree_;
  ScopeLayer* current_layer_ = nullptr;
  Symbol current_label_;

  std::unordered_map<Symbol, std::shared_ptr<ClassType>> class_types_;
  bool is_inside_method_ = false;

  std::ofstream out_;
};

} // namespace ast