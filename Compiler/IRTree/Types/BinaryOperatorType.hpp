#pragma once

#include <string>

namespace irt {

enum class BinaryOperatorType : char {
  PLUS,
  MINUS,
  MUL,
  DIV,
  MOD,
  AND,
  OR
};

namespace {

std::string ToString(BinaryOperatorType type) {
  switch (type) {
    case BinaryOperatorType::PLUS:return "PLUS";
    case BinaryOperatorType::MINUS:return "MINUS";
    case BinaryOperatorType::MUL:return "MUL";
    case BinaryOperatorType::DIV:return "DIV";
    case BinaryOperatorType::MOD:return "MOD";
    case BinaryOperatorType::AND:return "AND";
    default:return "OR";
  }
}

}

}