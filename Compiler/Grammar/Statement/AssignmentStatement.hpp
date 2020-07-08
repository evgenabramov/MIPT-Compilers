#pragma once

#include "Expression.hpp"
#include "Statement.hpp"
#include "NamedEntity.hpp"

namespace ast {

class AssignmentStatement : public Statement {
 public:
  AssignmentStatement(NamedEntity* named_entity, Expression* expression)
      : named_entity_(named_entity), expression_(expression) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  NamedEntity* GetNamedEntity() const {
    return named_entity_;
  }

  void SetNamedEntity(NamedEntity* named_entity) {
    named_entity_ = named_entity;
  }

  Expression* GetExpression() const {
    return expression_;
  }

  void SetExpression(Expression* expression) {
    expression_ = expression;
  }

 private:
  NamedEntity* named_entity_;
  Expression* expression_;
};

} // namespace ast