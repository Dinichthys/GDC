#pragma once

#include <string>
#include <unordered_map>

namespace Tests {

enum OpType { ADD, CONV, GEMM, MUL, MAT_MUL, MAX_POOL, RELU };

struct OpContext {
  std::string OpName;

  std::string FileName;

  size_t InputMinNum;
  size_t InputMaxNum;
  size_t OutputMinNum;
  size_t OutputMaxNum;
};

static const std::unordered_map<OpType, OpContext> MAP_OP_CONTEXT{
    {ADD,
     {.OpName = "Add",
      .FileName = "models/Add.onnx",
      .InputMinNum = 2,
      .InputMaxNum = 2,
      .OutputMinNum = 1,
      .OutputMaxNum = 1}},
    {CONV,
     {.OpName = "Conv",
      .FileName = "models/ConvWithPadding.onnx",
      .InputMinNum = 2,
      .InputMaxNum = 3,
      .OutputMinNum = 1,
      .OutputMaxNum = 1}},
    {GEMM,
     {.OpName = "Gemm",
      .FileName = "models/GemmAllAtributes.onnx",
      .InputMinNum = 2,
      .InputMaxNum = 3,
      .OutputMinNum = 1,
      .OutputMaxNum = 1}},
    {MUL,
     {.OpName = "Mul",
      .FileName = "models/MulUINT64.onnx",
      .InputMinNum = 2,
      .InputMaxNum = 2,
      .OutputMinNum = 1,
      .OutputMaxNum = 1}},
    {MAT_MUL,
     {.OpName = "MatMul",
      .FileName = "models/MatMul4D.onnx",
      .InputMinNum = 2,
      .InputMaxNum = 2,
      .OutputMinNum = 1,
      .OutputMaxNum = 1}},
    {MAX_POOL,
     {.OpName = "MaxPool",
      .FileName = "models/MaxPool3D.onnx",
      .InputMinNum = 1,
      .InputMaxNum = 1,
      .OutputMinNum = 1,
      .OutputMaxNum = 2}},
    {RELU,
     {.OpName = "LeakyRelu",
      .FileName = "models/LeakyRelu.onnx",
      .InputMinNum = 1,
      .InputMaxNum = 1,
      .OutputMinNum = 1,
      .OutputMaxNum = 1}},
};

}; // namespace Tests
