#pragma once
#include <vector>
// It's shit right now, that's minimal (almost) usable draft version.
namespace GiantGraph {  
class Operation;

// Tensor information holder
class ValueType {};
class Value { // It's not used yet, but probably will act as
private:      // interface for both OpOperand and OpResult classes.
  Operation *Op;
  ValueType Type;
public:

  Value() = default;
  // Not explicit because apparently, values are not always
  // results of operations (at least it is this way in mlir)
  Value(Operation *Op_) : Op(Op_) {}
  ~Value() = default;

  ValueType &getType() { return Type; }
  Operation *getDefiningOp() const { return Op; };
};

class OpResult;

class OpOperand {
private:
    Operation* Owner;
    OpResult* Value;
public:
  OpOperand() = default;
  OpOperand(Operation* Owner_) : Owner(Owner_) {}
  const Operation* getOwner() const { return Owner; }
  void setOwner(Operation* NewOwner) { Owner = NewOwner; }
  OpResult* getValue() { return Value; } // make const?
  void setValue(OpResult* NewValue) { Value = NewValue; }
};

class OpResult {
private:
  Operation* Owner;
  // for SSA
  unsigned Index;

  std::vector<OpOperand*> Uses;

public:
  ~OpResult() = default;
  explicit OpResult(unsigned Index_) : Index(Index_) {}
  void setOwner(Operation* NewOwner) { Owner = NewOwner; }
  Operation* getOwner() const { return Owner; }

  void setIndex(unsigned NewIndex) { Index = NewIndex; }
  unsigned getIndex() const { return Index; }
  const std::vector<OpOperand*> &getUses() const { return Uses; }

  void addUse(OpOperand* NewUse);
  void removeUse(OpOperand* NewUse);
};
} // namespace GiantIR
