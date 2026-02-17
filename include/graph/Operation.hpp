#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "Value.hpp"


namespace GiantGraph {
class Attribute {};

class Operation {
public:
  struct OperationName {
    std::string Name;
  };

private:
  OperationName OpName;

  std::vector<OpOperand> Operands;
  std::vector<OpResult> Results;

  //REVIEW -  maybe need smth similar to std::vector<std::string, Attribute> Attributes;
  std::vector<Attribute> Attributes;
public:
  Operation(OperationName Name, std::vector<OpOperand> Operands_,
            std::vector<OpResult> Results_)
      : OpName(std::move(Name)), Operands(std::move(Operands_)),
        Results(std::move(Results_)) {
    for (auto &Res : Results) {
      Res.setOwner(this);
    }
    for (auto &Op : Operands) {
      Op.setOwner(this);
    }
  }

  const std::string &getName() const { return OpName.Name; }

  OpOperand getOperandValue(size_t Num) const {
    assert(Num < Operands.size());
    return Operands[Num];
  }

  OpOperand& getOperandRef(size_t Num) {
    assert(Num < Operands.size());
    return Operands[Num];
  }

  const OpOperand& getOperandRef(size_t Num) const {
    assert(Num < Operands.size());
    return Operands[Num];
  }


  size_t getNumOperands() const { return Operands.size(); }

  OpResult getResultValue(size_t Num) const {
    assert(Num < Operands.size());
    return Results[Num];
  }

  OpResult& getResultRef(size_t Num) {
    assert(Num < Results.size());
    return Results[Num];
  }
  const OpResult& getResultRef(size_t Num) const {
    assert(Num < Results.size());
    return Results[Num];
  }

  size_t getNumResults() const { return Results.size(); }

  void dump() const { std::cout << "Operation: " << OpName.Name << "\n"; }
};
} // namespace GiantGraph
