#pragma once

#include "TreeNode.hpp"
#include "MainClass.hpp"
#include "ClassDeclarationList.hpp"

namespace ast {

class Program : public TreeNode {
 public:
  explicit Program(MainClass* main_class, ClassDeclarationList* class_declaration_list)
      : main_class_(main_class), class_declaration_list_(class_declaration_list) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  MainClass* GetMainClass() const {
      return main_class_;
  }

  ClassDeclarationList* GetClassDeclarationList() const {
      return class_declaration_list_;
  }

 private:
  MainClass* main_class_;
  ClassDeclarationList* class_declaration_list_;
};

} // namespace ast