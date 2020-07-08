#pragma once

#include "NamedEntity.hpp"

#include <string>

namespace ast {

class NamedArrayElement : public NamedEntity {
 public:
  explicit NamedArrayElement(std::string array_name, Expression* index_expression)
      : array_name_(std::move(array_name)), index_expression_(index_expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  const std::string& GetName() const override {
    return array_name_;
  }

  Expression* GetIndexExpression() const {
    return index_expression_;
  }

 private:
  std::string array_name_;
  Expression* index_expression_;
};

} // namespace ast