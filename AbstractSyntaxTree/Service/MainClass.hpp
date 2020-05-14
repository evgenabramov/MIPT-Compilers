#pragma once

#include "StatementList.hpp"
#include "TreeNode.hpp"

#include <string>
#include <memory>

namespace ast {

class MainClass : public TreeNode {
 public:
  MainClass(std::string name, StatementList* statements) : name_(std::move(name)), statements_(statements) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  std::string GetName() const {
      return name_;
  }

  StatementList* GetStatements() const {
      return statements_;
  }

 private:
  std::string name_;
  StatementList* statements_;
};

} // namespace ast