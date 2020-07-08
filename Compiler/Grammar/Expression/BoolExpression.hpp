#pragma once

#include "Expression.hpp"

namespace ast {

class BoolExpression : public Expression {
 public:
    explicit BoolExpression(const bool& value) : value_(value) {}

    void Accept(Visitor* visitor) override {
        visitor->Visit(this);
    }

    int GetValue() const {
        return static_cast<int>(value_);
    }

 private:
    bool value_ = 0;
};

} // namespace ast