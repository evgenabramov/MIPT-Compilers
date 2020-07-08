#pragma once

#include "Statement.hpp"
#include "VariableDeclaration.hpp"

namespace ast {

class VariableDeclarationStatement : public Statement {
 public:
  explicit VariableDeclarationStatement(VariableDeclaration* variable_declaration) : variable_declaration_(
      variable_declaration) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  VariableDeclaration* GetVariableDeclaration() const {
      return variable_declaration_;
  }

  void SetVariableDeclaration(VariableDeclaration* variable_declaration) {
      variable_declaration_ = variable_declaration;
  }

 private:
  VariableDeclaration* variable_declaration_;
};

} // namespace ast