#pragma once

#include <string>

namespace irt {

class Temporary {
 public:
  Temporary();

  explicit Temporary(const std::string& name);

  std::string ToString() const;

 private:
  std::string name_;
  static int counter_;
};

}

