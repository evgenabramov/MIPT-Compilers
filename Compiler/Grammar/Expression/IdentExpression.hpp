#pragma once

#include "Expression.hpp"

#include <string>

namespace ast {

class IdentExpression : public Expression {
 public:
  explicit IdentExpression(std::string identifier) : identifier_(std::move(identifier)) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  const std::string& GetIdentifier() const {
    return identifier_;
  }

 private:
  std::string identifier_;
};

} // namespace ast