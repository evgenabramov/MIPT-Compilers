#pragma once

#include "Symbol.hpp"
#include "ClassDeclaration.hpp"
#include "PrimitiveType.hpp"
#include "MethodType.hpp"

#include <unordered_map>
#include <memory>

namespace ast {

class ClassStorage {
 public:
  static ClassStorage& GetInstance() {
    static ClassStorage storage;
    return storage;
  }

  explicit ClassStorage(const ClassStorage& other) = delete;
  ClassStorage& operator=(const ClassStorage& other) = delete;

  void AddField(const Symbol& symbol, std::unordered_map<Symbol, std::shared_ptr<PrimitiveType>> field) {
    class_fields_[symbol] = std::move(field);
  }

  void AddMethod(const Symbol& symbol, std::unordered_map<Symbol, std::shared_ptr<MethodType>> method) {
    class_methods_[symbol] = std::move(method);
  }

  std::unordered_map<Symbol, std::shared_ptr<PrimitiveType>>& GetField(const Symbol& symbol) {
    return class_fields_[symbol];
  }

  std::unordered_map<Symbol, std::shared_ptr<MethodType>>& GetMethod(const Symbol& symbol) {
    return class_methods_[symbol];
  }

 private:
  ClassStorage() = default;
  ~ClassStorage() = default;

  // class name -> method name
  std::unordered_map<Symbol, std::unordered_map<Symbol, std::shared_ptr<PrimitiveType>>> class_fields_;
  std::unordered_map<Symbol, std::unordered_map<Symbol, std::shared_ptr<MethodType>>> class_methods_;
};

} // namespace ast