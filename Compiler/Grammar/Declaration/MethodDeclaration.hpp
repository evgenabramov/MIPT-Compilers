#pragma once

#include "FormalList.hpp"
#include "StatementList.hpp"
#include "Type.hpp"

namespace ast {

class MethodDeclaration : public Declaration {
 public:
  MethodDeclaration(Type* type, std::string identifier, FormalList* formal_list, StatementList* statement_list)
      : type_(type), identifier_(std::move(identifier)), formal_list_(formal_list), statement_list_(statement_list) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Type* GetType() const {
    return type_;
  }

  const std::string& GetIdentifier() const {
    return identifier_;
  }

  FormalList* GetFormalList() const {
    return formal_list_;
  }

  StatementList* GetStatementList() const {
    return statement_list_;
  }

 private:
  Type* type_;
  std::string identifier_;
  FormalList* formal_list_; // function arguments
  StatementList* statement_list_;
};

} // namespace ast