#pragma once

#include "Statement.hpp"
#include "ArrayDeclaration.hpp"

namespace ast {

class ArrayDeclarationStatement : public Statement {
 public:
  explicit ArrayDeclarationStatement(ArrayDeclaration* array_declaration) : array_declaration_(
      array_declaration) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  ArrayDeclaration* GetArrayDeclaration() const {
      return array_declaration_;
  }

  void SetArrayDeclaration(ArrayDeclaration* array_declaration) {
      array_declaration_ = array_declaration;
  }

 private:
  ArrayDeclaration* array_declaration_;
};

} // namespace ast