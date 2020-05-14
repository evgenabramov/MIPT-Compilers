#pragma once

#include "Expression.hpp"

#include <string>

namespace ast {

class IdentExpression : public Expression {
 public:
  explicit IdentExpression(std::string ident) : ident_(std::move(ident)) {}
    
  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  const std::string& GetIdentifier() const {
      return ident_;
  }

 private:
  std::string ident_;
};

} // namespace ast