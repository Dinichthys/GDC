#pragma once

#include "graph/Op.hpp"
#include <memory>
#include <string>

// This must be generated
namespace GiantGraph {

class Add : Op<Add> {
public:
  explicit Add(Operation* Oper) : Op<Add>(Oper) {}
  ~Add() = default;

  static std::string getOperationName() {
      return "arith.addi";
  }

  OpOperand getLhs() {
    return Oper->getOperandValue(0);
  }

  OpOperand getRhs() {
    return Oper->getOperandValue(1);
  }

  OpResult getResult() {
    return Oper->getResultValue(0);
  }

  bool verify() const {
    if (Oper->getName() != getOperationName()) {
      return false;
    }
    if (Oper->getNumOperands() != 2) {
      return false;
    }
    if (Oper->getNumResults() != 1) {
      return false;
    }
    return true;
  }
};

} // namespace GiantGraph
