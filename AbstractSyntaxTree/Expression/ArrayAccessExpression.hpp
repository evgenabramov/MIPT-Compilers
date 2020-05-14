#pragma once

#include "Expression.hpp"

#include <memory>

namespace ast {

class ArrayAccessExpression : public Expression {
 public:
  ArrayAccessExpression(std::string array_name, Expression* expression)
      : array_name_(std::move(array_name)), expression_(expression) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  const std::string& GetArrayName() const {
      return array_name_;
  }

  Expression* GetExpression() const {
      return expression_;
  }

 private:
  std::string array_name_;
  Expression* expression_;
};

} // namespace ast