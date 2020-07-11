#pragma once

#include <string>

#include "TreeNode.hpp"
#include "ExpressionList.hpp"

namespace ast {

class MethodInvocation : public TreeNode {
 public:
  MethodInvocation(Expression* expression, std::string identifier) : expression_(expression), identifier_(identifier) {}

  MethodInvocation(Expression* expression, std::string identifier, ExpressionList* expression_list) : expression_(
      expression), identifier_(std::move(identifier)), expression_list_(expression_list) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* GetExpression() const {
    return expression_;
  }

  const std::string& GetIdentifier() const {
    return identifier_;
  }

  ExpressionList* GetExpressionList() const {
    return expression_list_;
  }

 private:
  Expression* expression_;
  std::string identifier_;
  ExpressionList* expression_list_ = nullptr;
};

} // namespace ast