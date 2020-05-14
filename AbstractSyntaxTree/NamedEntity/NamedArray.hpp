#pragma once

#include "NamedEntity.hpp"

#include <string>

namespace ast {

class NamedArray : public NamedEntity {
 public:
  explicit NamedArray(std::string name) : name_(std::move(name)) {}

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