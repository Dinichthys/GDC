#include "MLIR/BuildMLIR.hpp"

#include <unordered_map>

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Target/LLVMIR/Dialect/All.h"

#include "graph/Graph.hpp"
#include "graph/Operation.hpp"

namespace MLIRBuild {

static const std::string ConvOpName = "Conv";
static const std::string GemmOpName = "Gemm";
static const std::string AddOpName = "Add";
static const std::string ReluOpName = "Relu";
static const std::string MulOpName = "Mul";
static const std::string MatMulOpName = "MatMul";
static const std::string MaxPoolOpName = "MatMul";

struct MLIRInstructionInfo {
  bool used;
  mlir::Operation *op;
};

mlir::ModuleOp buildModule(GiantGraph::GiantGraph &Graph) {
  mlir::MLIRContext Context;
  mlir::registerLLVMDialectTranslation(Context);
  Context.loadDialect<mlir::func::FuncDialect>();
  Context.loadDialect<mlir::arith::ArithDialect>();

  mlir::OpBuilder Builder(&Context);
  auto Location = Builder.getUnknownLoc();

  auto Module = Builder.create<mlir::ModuleOp>(Location);

  auto FuncType = Builder.getFunctionType({}, {});

  auto Func = Builder.create<mlir::func::FuncOp>(Location, ComputeGraphFuncName,
                                                 FuncType);
  auto *Entry = Func.addEntryBlock();
  Builder.setInsertionPointToStart(Entry);

  std::unordered_map<GiantGraph::Operation *, MLIRInstructionInfo> IsUsed;

  size_t UsedNum = 0;
  while (UsedNum < Graph.nodes().size()) {
    for (auto &Op : Graph.nodes()) {
      auto It = IsUsed.find(Op.get());
      if (It == IsUsed.end()) {
        IsUsed.insert({Op.get(), {false, nullptr}});
        It = IsUsed.find(Op.get());
      }

      bool AllOperandsUsed = true;
      size_t Size = Op->getNumOperands();
      std::vector<mlir::Operation *> Operands{Size, nullptr};
      for (size_t I = 0; I < Size; I++) {
        auto &Param = Op->getOperandRef(I);
        auto ParamIt = IsUsed.find(Param.getValue()->getOwner());
        if (ParamIt == IsUsed.end()) {
          AllOperandsUsed = false;
          IsUsed.insert({Param.getValue()->getOwner(), {false, nullptr}});
          break;
        }
        if (!ParamIt->second.used) {
          AllOperandsUsed = false;
          break;
        }

        Operands.push_back(ParamIt->second.op);
      }

      if (!AllOperandsUsed) {
        continue;
      }

      auto &Name = Op->getName();
      if (Name == ConvOpName) {
        It->second.op = Builder.create<mlir::arith::Conv>(Location, Operands);
      } else if (Name == GemmOpName) {
        It->second.op = Builder.create<mlir::arith::AddFOp>(Location, Operands);
      } else if (Name == AddOpName) {
        It->second.op = Builder.create<mlir::arith::AddFOp>(Location, Operands);
      } else if (Name == ReluOpName) {
        It->second.op = Builder.create<mlir::arith::AddFOp>(Location, Operands);
      } else if (Name == MulOpName) {
        It->second.op = Builder.create<mlir::arith::AddFOp>(Location, Operands);
      } else if (Name == MatMulOpName) {
        It->second.op = Builder.create<mlir::arith::AddFOp>(Location, Operands);
      } else if (Name == MaxPoolOpName) {
        It->second.op = Builder.create<mlir::arith::AddFOp>(Location, Operands);
      } else {
        throw std::runtime_error("Invalid Operation");
      }
      It->second.used = true;
      UsedNum++;
    }
  }

  Builder.create<mlir::func::ReturnOp>(Location);

  if (mlir::failed(Module.verify())) {
    throw std::runtime_error("Error in verification MLIR Module\n");
  }

  return Module;
}

} // namespace MLIRBuild
