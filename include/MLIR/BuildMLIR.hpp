#pragma once

#include <string>

#include "mlir/IR/BuiltinOps.h"

#include "graph/Graph.hpp"

namespace MLIRBuild {

std::string ComputeGraphFuncName = "ComputeGraph";

mlir::ModuleOp buildModule(GiantGraph::GiantGraph &graph);

}
