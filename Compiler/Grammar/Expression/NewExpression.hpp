#pragma once

#include "Expression.hpp"

#include <string>

namespace ast {

class NewExpression : public Expression {
 public:
  explicit NewExpression(const std::string& identifier) : identifier_(identifier) {}

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