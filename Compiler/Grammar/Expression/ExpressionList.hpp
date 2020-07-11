#pragma once

#include "TreeNode.hpp"

namespace ast {

class ExpressionList : public TreeNode {
 public:
  ExpressionList() = default;

  ExpressionList(Expression* first_item, ExpressionList* tail) : first_item_(first_item), tail_(tail) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Expression* GetFirstItem() const {
    return first_item_;
  }

  void SetFirstItem(Expression* first_item) {
    first_item_ = first_item;
  }

  ExpressionList* GetTail() const {
    return tail_;
  }

  void SetTail(ExpressionList* tail) {
    tail_ = tail;
  }

 private:
  Expression* first_item_ = nullptr;
  ExpressionList* tail_ = nullptr;
};

} // namespace ast