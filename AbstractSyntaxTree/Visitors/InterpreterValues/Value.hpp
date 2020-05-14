#pragma once

namespace ast {

class Value {
 public:
  virtual int toInt() const = 0;
  virtual bool toBool() const = 0;

  virtual std::string GetTypeIdentifier() const = 0;
};

} // namespace ast