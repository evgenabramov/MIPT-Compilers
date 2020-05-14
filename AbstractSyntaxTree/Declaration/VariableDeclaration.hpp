#pragma once

#include "Declaration.hpp"
#include "SimpleType.hpp"

#include <string>

namespace ast {

class VariableDeclaration : public Declaration {
 public:
  VariableDeclaration(SimpleType* simple_type, std::string variable_name)
      : simple_type_(simple_type), variable_name_(std::move(variable_name)) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  SimpleType* GetSimpleType() const {
      return simple_type_;
  }

  void SetSimpleType(SimpleType* simple_type) {
      simple_type_ = simple_type;
  }

  const std::string& GetVariableName() const {
      return variable_name_;
  }

  void SetVariableName(const std::string& variable_name) {
      variable_name_ = variable_name;
  }

 private:
  SimpleType* simple_type_;
  std::string variable_name_;
};

} // namespace ast