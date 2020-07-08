#pragma once

#include "Expression.hpp"

namespace ast {

class OrExpression : public Expression {
 public:
    OrExpression(Expression* left, Expression* right) : left_(left), right_(right) {}

    void Accept(Visitor* visitor) override {
      visitor->Visit(this);
    }

    Expression* GetLeftExpression() const {
       return left_;
    }

    Expression* GetRightExpression() const {
          return right_;
    }

 private:
    Expression* left_;
    Expression* right_;
};

} // namespace ast