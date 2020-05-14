#pragma once

#include "TreeNode.hpp"
#include "MainClass.hpp"

namespace ast {

class Program : public TreeNode {
 public:
  explicit Program(MainClass* main_class) : main_class_(main_class) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  MainClass* GetMainClass() const {
      return main_class_;
  }

  void SetMainClass(MainClass* main_class) {
      main_class_ = main_class;
  }

 private:
  MainClass* main_class_;
};

} // namespace ast