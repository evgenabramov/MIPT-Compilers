#pragma once

#include "Visitor.hpp"

namespace ast {

// Node for tree traversal
// https://github.com/akhtyamovpavel/CompilersCourse/blob/master/04-visitors/base_elements/BaseElement.h
class TreeNode {
 public:
    virtual void Accept(Visitor* visitor) = 0;
    virtual ~TreeNode() = default;
};

} // namespace ast