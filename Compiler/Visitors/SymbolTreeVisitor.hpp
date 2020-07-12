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

  void Visit(NewExpression* new_expression) override {}

  void Visit(ExpressionList* expression_list) override {}

  void Visit(ArrayLengthExpression* array_length_expression) override {}

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

  void Visit(ThisExpression* this_expression) override {}

  void Visit(MethodInvocationExpression* method_invocation_expression) override {
    method_invocation_expression->GetMethodInvocation()->Accept(this);
  }

  void Visit(MethodInvocation* method_invocation) override {
    // Check if method exists
    scope_layer_tree_->GetScopeLayer(method_invocation->GetIdentifier());
    method_invocation->GetExpressionList()->Accept(this);
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
    if_statement->GetStatement()->Accept(this);
  }

  void Visit(PrintStatement* print_statement) override {}

  void Visit(ReturnStatement* return_statement) override {}

  void Visit(VariableDeclarationStatement* variable_declaration_statement) override {
    variable_declaration_statement->GetVariableDeclaration()->Accept(this);
  }

  void Visit(WhileStatement* while_statement) override {
    while_statement->GetStatement()->Accept(this);
  }

  void Visit(AssertStatement* assert_statement) override {}

  void Visit(IfElseStatement* if_else_statement) override {
    if_else_statement->GetFirstStatement()->Accept(this);
    if_else_statement->GetSecondStatement()->Accept(this);
  }

  void Visit(ScopeStatement* scope_statement) override {
    current_layer_ = new ScopeLayer(current_layer_);
    scope_statement->GetStatementList()->Accept(this);
    current_layer_ = current_layer_->GetParent();
  }

  void Visit(MethodInvocationStatement* method_invocation_statement) override {
    method_invocation_statement->GetMethodInvocation()->Accept(this);
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
    out_ << "-------------------------------------------" << std::endl;
    out_ << "Scope Tree Layers info:" << std::endl;
    scope_layer_tree_->GetRoot()->PrintLayer(out_, 0);
    out_ << "-------------------------------------------" << std::endl;
  }

  ScopeLayerTree* GetScopeLayerTree() const {
    return scope_layer_tree_;
  }

  void OutputClassesInfo() {
    out_ << "-------------------------------------------" << std::endl;
    out_ << "Classes info:" << std::endl;
    for (auto& [class_name, class_type] : class_types_) {
      out_ << "Class name: " << class_name.GetName() << std::endl;

      if (!class_type->GetMethodTypes().empty()) {
        out_ << "\tClass methods:" << std::endl;
        for (auto&[method_name, method_type] : class_type->GetMethodTypes()) {
          out_ << "\t\t[Method name:] " << method_name.GetName()
               << " [Return type:] " << method_type->GetReturnValueType()->GetIdentifier() << std::endl;
        }
      }

       if (!class_type->GetFieldTypes().empty()) {
         out_ << "\tClass fields:" << std::endl;
         for (auto& [field_name, field_type] : class_type->GetFieldTypes()) {
           out_ << "\t\t[Field name:] " << field_name.GetName()
                << " [Type:] ";
           if (field_type->IsSimpleType()) {
             out_ << field_type->GetTypeName() << std::endl;
           } else {
             out_ << field_type->GetTypeName() << "[]" << std::endl;
           }
         }
       }
    }
    out_ << "-------------------------------------------" << std::endl;
  }

  void FillClassStorage() {
    ClassStorage& class_storage = ClassStorage::GetInstance();
    for (auto& [class_name, class_type] : class_types_) {
      class_storage.SetClassFields(class_name, class_type->GetFieldTypes());
      class_storage.SetClassMethods(class_name, class_type->GetMethodTypes());
    }
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