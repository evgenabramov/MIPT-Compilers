#pragma once

#include "Statement.hpp"
#include "MethodInvocation.hpp"

namespace ast {

class MethodInvocationStatement : public Statement {
 public:
  explicit MethodInvocationStatement(MethodInvocation* method_invocation) : method_invocation_(method_invocation) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  MethodInvocation* GetMethodInvocation() const {
    return method_invocation_;
  }

 private:
  MethodInvocation* method_invocation_;
};

} // namespace ast