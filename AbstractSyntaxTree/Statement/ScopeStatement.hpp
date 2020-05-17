#pragma once

#include "Statement.hpp"

namespace ast {

class ScopeStatement : public Statement {
 public:
  explicit ScopeStatement(StatementList* statement_list) : statement_list_(statement_list) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  StatementList* GetStatementList() const {
      return statement_list_;
  }

 private:
  StatementList* statement_list_;
};

} // namespace ast