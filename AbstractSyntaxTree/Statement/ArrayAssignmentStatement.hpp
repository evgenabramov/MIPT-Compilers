#pragma once

#include "Expression.hpp"
#include "Statement.hpp"
#include "NamedArray.hpp"
#include "SimpleType.hpp"

namespace ast {

class ArrayAssignmentStatement : public Statement {
 public:
  ArrayAssignmentStatement(NamedArray* named_array, SimpleType* simple_type, Expression* expression)
      : named_array_(named_array), simple_type_(simple_type), expression_(expression) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  NamedArray* GetNamedArray() const {
      return named_array_;
  }

  void SetNamedArray(NamedArray* named_array) {
      named_array_ = named_array;
  }

  SimpleType* GetSimpleType() const {
      return simple_type_;
  }

  void SetSimpleType(SimpleType* simple_type) {
      simple_type_ = simple_type;
  }

  Expression* GetExpression() const {
      return expression_;
  }

  void SetExpression(Expression* expression) {
      expression_ = expression;
  }

 private:
  NamedArray* named_array_;
  SimpleType* simple_type_;
  Expression* expression_;
};

} // namespace ast