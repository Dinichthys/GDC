#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace GiantGraph {

class Value {};

class Attribute {};

class Operation {
public:
  struct OperationName {
    std::string Name;
  };

private:
  OperationName OpName;

  std::vector<Value> Input;
  std::vector<Value> Output;
  std::vector<Attribute> Attributes;

public:
  Operation(OperationName Name, std::vector<Value> Inp, size_t OutputSize)
      : OpName(std::move(Name)), Input(std::move(Inp)), Output(OutputSize) {}

  const std::string &getName() const { return OpName.Name; }

  size_t getNumOutput() const { return Output.size(); }

  Value getInput(size_t Num) const {
    assert(Num < Input.size());
    return Input[Num];
  }

  size_t getNumInput() const { return Input.size(); }

  Value getOutput(size_t Num) {
    assert(Num < Output.size());
    return Output[Num];
  }

  void dump() const { std::cout << "Operation: " << OpName.Name << "\n"; }
};
} // namespace GiantGraph
