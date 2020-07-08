#pragma once

#include "NamedEntity.hpp"

#include <string>

namespace ast {

class NamedVariable : public NamedEntity {
 public:
  explicit NamedVariable(std::string name) : name_(std::move(name)) {}

  void Accept(Visitor* visitor) override {
      visitor->Visit(this);
  }

  const std::string& GetName() const override {
      return name_;
  }

 private:
  std::string name_;
};

} // namespace ast