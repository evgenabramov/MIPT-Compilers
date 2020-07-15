#pragma once

#include "Symbol.hpp"
#include "ClassDeclaration.hpp"
#include "PrimitiveType.hpp"
#include "MethodType.hpp"

#include <unordered_map>
#include <memory>

#include <iostream>

namespace ast {

using std::shared_ptr;
using std::unordered_map;

class ClassStorage {
 public:
  static ClassStorage& GetInstance() {
    static ClassStorage storage;
    return storage;
  }

  explicit ClassStorage(const ClassStorage& other) = delete;
  ClassStorage& operator=(const ClassStorage& other) = delete;

  void SetClassFields(const Symbol& symbol, unordered_map<Symbol, shared_ptr<PrimitiveType>> field) {
    class_fields_[symbol] = std::move(field);
  }

  void SetClassMethods(const Symbol& symbol, unordered_map<Symbol, shared_ptr<MethodType>> method) {
    class_methods_[symbol] = std::move(method);
  }

  unordered_map<Symbol, shared_ptr<PrimitiveType>>& GetClassFields(const Symbol& symbol) {
    return class_fields_[symbol];
  }

  unordered_map<Symbol, shared_ptr<MethodType>>& GetClassMethods(const Symbol& symbol) {
    return class_methods_[symbol];
  }

  void Output() {
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Class storage: " << std::endl;

    if (!class_methods_.empty()) {
      std::cout << "Methods info: " << std::endl;
    }

    for (auto&[class_name, class_methods] : class_methods_) {
      if (class_methods.empty()) {
        continue;
      }
      std::cout << "\tClass name: " << class_name.GetName() << std::endl;
      for (auto&[method_name, method_type] : class_methods) {
        std::cout << "\t\t[Method name:] " << method_name.GetName()
                  << " [Method return type:] " << method_type->GetReturnValueType()->GetIdentifier() << std::endl;
      }
    }

    if (!class_fields_.empty()) {
      std::cout << "Fields info: " << std::endl;
    }

    for (auto&[class_name, class_fields] : class_fields_) {
//      if (class_fields.empty()) {
//        continue;
//      }
      std::cout << "\tClass name: " << class_name.GetName() << std::endl;
      for (auto&[field_name, field_type] : class_fields) {
        std::cout << "\t\t[Field name:] " << field_name.GetName()
                  << " [Field type:] " << field_type->GetTypeName() << std::endl;
      }
    }
    std::cout << "-------------------------------------------" << std::endl;
  }

 private:
  ClassStorage() = default;
  ~ClassStorage() = default;

  // class name -> field/method name
  unordered_map<Symbol, unordered_map<Symbol, shared_ptr<PrimitiveType>>> class_fields_;
  unordered_map<Symbol, unordered_map<Symbol, shared_ptr<MethodType>>> class_methods_;
};

} // namespace ast