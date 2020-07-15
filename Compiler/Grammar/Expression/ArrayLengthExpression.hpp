#pragma once

namespace ast {

class ArrayLengthExpression : public Expression {
 public:
  explicit ArrayLengthExpression(Expression* expression) : expression_(expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* GetExpression() const {
    return expression_;
  }

 private:
  Expression* expression_;
};

} // namespace ast