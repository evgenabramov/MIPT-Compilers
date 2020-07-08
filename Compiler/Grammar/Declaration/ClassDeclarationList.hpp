#pragma once

#include "ClassDeclaration.hpp"

namespace ast {

class ClassDeclarationList : public TreeNode {
 public:
  ClassDeclarationList() = default;

  ClassDeclarationList(ClassDeclaration* first_item, ClassDeclarationList* tail)
    : first_item_(first_item), tail_(tail) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  ClassDeclaration* GetFirstItem() const {
      return first_item_;
  }

  ClassDeclarationList* GetTail() const {
      return tail_;
  }

 private:
  ClassDeclaration* first_item_ = nullptr;
  ClassDeclarationList* tail_ = nullptr;
};

} // namespace ast