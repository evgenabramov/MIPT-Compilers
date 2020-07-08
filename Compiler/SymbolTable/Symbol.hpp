#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace ast {

class Symbol {
 public:
  Symbol() = default;
  Symbol(std::string name) : name_(std::move(name)) {}
  Symbol(const char* name) : Symbol(std::string(name)) {}
  Symbol(const Symbol& other) : name_(other.name_) {}

  bool operator==(const Symbol& other) const {
    return name_ == other.name_;
  }

  bool operator!=(const Symbol& other) const {
    return name_ != other.name_;
  }

  std::string GetName() const {
    return name_;
  }

 private:
  std::string name_;
};

} // namespace ast

namespace std {
template<>
struct hash<ast::Symbol> {
  size_t operator()(const ast::Symbol& symbol) const {
    return std::hash<std::string>()(symbol.GetName());
  }
};
}