#pragma once

#include "TreeNode.hpp"
#include "Visitor.hpp"

namespace ast {

// Can be on the left side of assignment (lvalue)
class NamedEntity : public TreeNode {
 public:
    virtual ~NamedEntity() = default;
    void Accept(Visitor* visitor) override {
        visitor->Visit(this);
    }

    virtual const std::string& GetName() const = 0;
};

} // namespace ast