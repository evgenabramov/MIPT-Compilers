#pragma once

#include <string>

namespace irt {

class Temporary {
 public:
  Temporary();

  explicit Temporary(const std::string& name);
  Temporary(const Temporary& other) = default;

  std::string ToString() const;
  
  bool operator==(const Temporary& other) const;
  bool operator!=(const Temporary& other) const;

 private:
  std::string name_;
  static int counter_;
};

}

template<> struct std::hash<irt::Temporary> {
  std::size_t operator()(irt::Temporary const& temporary) const noexcept {
    return std::hash<std::string>{}(temporary.ToString());
  }
};

