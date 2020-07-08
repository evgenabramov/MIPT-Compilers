#pragma once

#include "Expression.hpp"
#include "SimpleType.hpp"

#include <memory>

namespace ast {

class NewArrayExpression : public Expression {
 public:
  NewArrayExpression(SimpleType* simple_type, Expression* expression)
      : simple_type_(simple_type), expression_(expression) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  SimpleType* GetSimpleType() const {
      return simple_type_;
  }

  Expression* GetExpression() const {
      return expression_;
  }

 private:
  SimpleType* simple_type_;
  Expression* expression_;
};

} // namespace ast