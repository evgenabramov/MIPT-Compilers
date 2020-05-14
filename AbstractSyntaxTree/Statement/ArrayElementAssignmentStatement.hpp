#pragma once

#include "Expression.hpp"
#include "Statement.hpp"
#include "NamedArray.hpp"

#include <string>
#include <memory>

namespace ast {

class ArrayElementAssignmentStatement : public Statement {
 public:
  ArrayElementAssignmentStatement(std::string array_name, Expression* index_expression, Expression* value_expression)
  : array_name_(std::move(array_name)), index_expression_(index_expression), value_expression_(value_expression) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  const std::string& GetArrayName() const {
      return array_name_;
  }

  Expression* GetIndexExpression() const {
      return index_expression_;
  }

  Expression* GetValueExpression() const {
      return value_expression_;
  }

 private:
  std::string array_name_;
  Expression* index_expression_;
  Expression* value_expression_;
};

} // namespace ast