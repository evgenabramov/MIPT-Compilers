#pragma once

#include "Formal.hpp"

namespace ast {

class FormalList : public TreeNode {
 public:
  FormalList() = default;
  FormalList(Formal* first_item, FormalList* tail) : first_item_(first_item), tail_(tail) {}

  void Accept(Visitor* visitor) override {
    visitor->Visit(this);
  }

  Formal* GetFirstItem() const {
    return first_item_;
  }

  FormalList* GetTail() const {
    return tail_;
  }

  size_t GetSize() const {
    if (first_item_ == nullptr) {
      return 0;
    } else if (tail_ == nullptr) {
      return 1;
    }
    return 1 + tail_->GetSize();
  }

 private:
  Formal* first_item_ = nullptr;
  FormalList* tail_ = nullptr;
};

} // namespace ast