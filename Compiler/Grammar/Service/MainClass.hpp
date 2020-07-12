#pragma once

#include "StatementList.hpp"
#include "TreeNode.hpp"

#include <string>
#include <memory>

namespace ast {

class MainClass : public TreeNode {
 public:
  MainClass(std::string name, StatementList* statement_list)
      : name_(std::move(name)), statement_list_(statement_list) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  std::string GetName() const {
    return name_;
  }

  StatementList* GetStatementList() const {
    return statement_list_;
  }

 private:
  std::string name_;
  StatementList* statement_list_;
};

} // namespace ast