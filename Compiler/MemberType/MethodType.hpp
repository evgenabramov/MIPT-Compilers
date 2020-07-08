#pragma once

#include "MemberType.hpp"
#include "MethodDeclaration.hpp"

namespace ast {

class MethodType : public MemberType {
 public:
  explicit MethodType(MethodDeclaration* method_declaration)
      : method_declaration_(method_declaration),
        formal_list_(method_declaration->GetFormalList()),
        return_value_type_(method_declaration->GetType()) {}

  MethodDeclaration* GetMethodDeclaration() const {
    return method_declaration_;
  }

  FormalList* GetFormalList() const {
    return formal_list_;
  }

  Type* GetReturnValueType() const {
    return return_value_type_;
  }

 private:
  MethodDeclaration* method_declaration_;
  FormalList* formal_list_; // for fast access
  Type* return_value_type_;
};

} // namespace ast