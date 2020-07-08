#pragma once

#include "Declaration.hpp"
#include "Type.hpp"

#include <string>

namespace ast {

class VariableDeclaration : public Declaration {
 public:
  VariableDeclaration(Type* type, std::string variable_name)
    : type_(type), variable_name_(std::move(variable_name)) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Type* GetType() const {
    return type_;
  }

  void SetType(Type* type) {
    type_ = type;
  }

  const std::string& GetVariableName() const {
    return variable_name_;
  }

  void SetVariableName(const std::string& variable_name) {
    variable_name_ = variable_name;
  }

 private:
  Type* type_;
  std::string variable_name_;
};

} // namespace ast