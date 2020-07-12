#pragma once

#include "Declaration.hpp"

#include <string>

namespace ast {

class ClassDeclaration : public TreeNode {
 public:
  ClassDeclaration(std::string class_name, DeclarationList* declaration_list)
    : class_name_(std::move(class_name)), declaration_list_(declaration_list) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  const std::string& GetClassName() const {
    return class_name_;
  }

  void SetClassName(const std::string& class_name) {
    class_name_ = class_name;
  }

  DeclarationList* GetDeclarationList() const {
    return declaration_list_;
  }

 private:
  std::string class_name_;
  DeclarationList* declaration_list_;
};

} // namespace ast