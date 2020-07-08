#pragma once

#include "ClassDeclaration.hpp"
#include "MemberType.hpp"
#include "MethodType.hpp"
#include "PrimitiveType.hpp"
#include "Symbol.hpp"

#include <unordered_map>

namespace ast {

class ClassType : public MemberType {
 public:
  explicit ClassType(ClassDeclaration* class_declaration) : class_declaration_(class_declaration) {}

  void AddField(const Symbol& symbol, std::shared_ptr<PrimitiveType> field) {
    field_types_[symbol] = std::move(field);
  }

  void AddMethod(const Symbol& symbol, std::shared_ptr<MethodType> method) {
    method_types_[symbol] = std::move(method);
  }

  ClassDeclaration* GetClassDeclaration() const {
    return class_declaration_;
  }

  const std::unordered_map<Symbol, std::shared_ptr<PrimitiveType>>& GetFieldTypes() const {
    return field_types_;
  }

  const std::unordered_map<Symbol, std::shared_ptr<MethodType>>& GetMethodTypes() const {
    return method_types_;
  }

 private:
  ClassDeclaration* class_declaration_;
  std::unordered_map<Symbol, std::shared_ptr<PrimitiveType>> field_types_;
  std::unordered_map<Symbol, std::shared_ptr<MethodType>> method_types_;
};

} // namespace ast