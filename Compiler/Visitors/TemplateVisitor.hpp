#pragma once

#include "TreeNode.hpp"
#include "Visitor.hpp"
#include "Object.hpp"

namespace ast {

template<typename T>
class TemplateVisitor : public Visitor {
 public:
  T Accept(TreeNode* tree_node) {
    tree_node->Accept(this);
    return tos_value_;
  }

 protected:
  T tos_value_;
};

template Object* TemplateVisitor<Object*>::Accept(TreeNode* tree_node);

} // namespace ast