#include "MLIR/BuildMLIR.hpp"

#include <unordered_map>

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Target/LLVMIR/Dialect/All.h"
#include "llvm/ADT/APFloat.h"

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

static size_t GetIdxOfOperandInOwnerRes(auto &Op, size_t OpIdx);

mlir::ModuleOp buildModule(GiantGraph::GiantGraph &Graph) {
  mlir::MLIRContext Context;
  mlir::registerLLVMDialectTranslation(Context);
  Context.loadDialect<mlir::func::FuncDialect>();
  Context.loadDialect<mlir::linalg::LinalgDialect>();
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
        size_t InputIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        size_t FilterIdx = GetIdxOfOperandInOwnerRes(Op, 1);

        mlir::ValueRange Input{Operands[0]->getResults()[InputIdx],
                               Operands[1]->getResults()[FilterIdx]};

        mlir::ValueRange Output{mlir::Value()};

        It->second.op = Builder.create<mlir::linalg::Conv2DNchwFchwOp>(
            Location, Input, Output);
      } else if (Name == GemmOpName) {
        size_t FirstIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        size_t SecondIdx = GetIdxOfOperandInOwnerRes(Op, 1);

        size_t ThirdIdx = GetIdxOfOperandInOwnerRes(Op, 2);

        mlir::ValueRange InputMatMul{Operands[0]->getResults()[FirstIdx],
                                     Operands[1]->getResults()[SecondIdx]};

        mlir::ValueRange OutputMatMul{mlir::Value()};

        auto OutInstMatMul = Builder.create<mlir::linalg::MatmulOp>(
            Location, InputMatMul, OutputMatMul);

        mlir::ValueRange InputGemm{OutInstMatMul->getResults()[0],
                                   Operands[1]->getResults()[ThirdIdx]};

        mlir::ValueRange OutputGemm{mlir::Value()};

        It->second.op = Builder.create<mlir::linalg::AddOp>(Location, InputGemm,
                                                            OutputGemm);
      } else if (Name == AddOpName) {
        size_t FirstIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        size_t SecondIdx = GetIdxOfOperandInOwnerRes(Op, 1);

        mlir::ValueRange Input{Operands[0]->getResults()[FirstIdx],
                               Operands[1]->getResults()[SecondIdx]};

        mlir::ValueRange Output{mlir::Value()};

        It->second.op =
            Builder.create<mlir::linalg::AddOp>(Location, Input, Output);
      } else if (Name == ReluOpName) {
        size_t FirstIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        mlir::Value InputVal = Operands[0]->getResults()[FirstIdx];

        mlir::ValueRange Input{InputVal};

        mlir::ValueRange Output{mlir::Value()};

        auto InputTy = InputVal.getType().cast<mlir::RankedTensorType>();
        auto ElemTy = InputTy.getElementType();
        unsigned Rank = InputTy.getRank();

        auto Empty = Builder.create<mlir::tensor::EmptyOp>(
            Location, InputTy.getShape(), ElemTy);

        auto ZeroAttr = Builder.getZeroAttr(ElemTy);
        auto Zero = Builder.create<mlir::arith::ConstantOp>(Location, ZeroAttr);

        auto IdMap = Builder.getMultiDimIdentityMap(Rank);
        auto IndexingMaps = Builder.getAffineMapArrayAttr({IdMap, IdMap});

        auto Ctx = Builder.getContext();

        llvm::SmallVector<mlir::Attribute> Iters(
            Rank, mlir::StringAttr::get(Ctx, "parallel"));
        auto IteratorTypes = Builder.getArrayAttr(Iters);

        It->second.op = Builder.create<mlir::linalg::GenericOp>(
            Location, InputTy, mlir::ValueRange{Input}, mlir::ValueRange{Empty},
            IndexingMaps, IteratorTypes,
            [&](mlir::OpBuilder &b, mlir::Location loc, mlir::ValueRange args) {
              mlir::Value InVal = args[0];
              mlir::Value Res;
              if (ElemTy.isF32() || ElemTy.isF16() || ElemTy.isF64()) {
                Res = b.create<mlir::arith::MaxFOp>(loc, Zero, InVal);
              } else {
                Res = b.create<mlir::arith::MaxSIOp>(loc, Zero, InVal);
              }
              b.create<mlir::linalg::YieldOp>(loc, Res);
            });
      } else if (Name == MulOpName) {
        size_t FirstIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        size_t SecondIdx = GetIdxOfOperandInOwnerRes(Op, 1);

        mlir::ValueRange Input{Operands[0]->getResults()[FirstIdx],
                               Operands[1]->getResults()[SecondIdx]};

        mlir::ValueRange Output{mlir::Value()};

        It->second.op =
            Builder.create<mlir::linalg::MulOp>(Location, Input, Output);
      } else if (Name == MatMulOpName) {
        size_t FirstIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        size_t SecondIdx = GetIdxOfOperandInOwnerRes(Op, 1);

        mlir::ValueRange Input{Operands[0]->getResults()[FirstIdx],
                               Operands[1]->getResults()[SecondIdx]};

        mlir::ValueRange Output{mlir::Value()};

        It->second.op =
            Builder.create<mlir::linalg::MatmulOp>(Location, Input, Output);
      } else if (Name == MaxPoolOpName) {
        size_t FirstIdx = GetIdxOfOperandInOwnerRes(Op, 0);

        mlir::ValueRange Input{Operands[0]->getResults()[FirstIdx]};

        mlir::RankedTensorType OutTy;

        auto ElemTy = OutTy.getElementType();

        auto Empty = Builder.create<mlir::tensor::EmptyOp>(Location, OutTy);

        double NegInf = std::numeric_limits<double>::lowest();
        if (ElemTy.isF32())
          NegInf = std::numeric_limits<float>::lowest();
        auto NegInfAttr = Builder.getFloatAttr(ElemTy, NegInf);
        auto NegInfVal =
            Builder.create<mlir::arith::ConstantOp>(Location, NegInfAttr);
        auto Init =
            Builder.create<mlir::linalg::FillOp>(Location, NegInfVal, Empty)
                .getResult(0);

        It->second.op = Builder.create<mlir::linalg::PoolingNhwcMaxOp>(
            Location, OutTy, Input, mlir::ValueRange{Init});
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

static size_t GetIdxOfOperandInOwnerRes(auto &Op, size_t OpIdx) {
  auto *Ptr = Op->getOperandRef(OpIdx).getValue();
  size_t Idx = 0;
  auto *PtrOwner = Ptr->getOwner();
  size_t OwnerResNum = PtrOwner->getNumResults();
  for (size_t I = 0; I < OwnerResNum; I++) {
    if (&PtrOwner->getResultRef(I) == Ptr) {
      Idx = I;
      break;
    }
  }

  return Idx;
}

} // namespace MLIRBuild
