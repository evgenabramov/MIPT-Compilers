#pragma once

#include "TreeNode.hpp"
#include "Declaration.hpp"

namespace ast {

class DeclarationList : public TreeNode {
 public:
    DeclarationList() = default;
    DeclarationList(Declaration* first, DeclarationList* tail) : first_(first), tail_(tail) {}

    void Accept(Visitor* visitor) override {
        visitor->Visit(this);
    }

    Declaration* GetFirstItem() const {
        return first_;
    }

    DeclarationList* GetTail() const {
        return tail_;
    }

 private:
    Declaration* first_;
    DeclarationList* tail_;
};

} // namespace ast