#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"
#include "graph/GraphVizard.hpp"

#include "MLIR/BuildMLIR.hpp"

#include "mlir/Target/LLVMIR/Export.h"
#include "llvm/IR/Module.h"

int main(int argc, char **argv) {
  CLI::App CLIApp;
  std::string ModelFileName;
  CLIApp.add_option("-f,--file", ModelFileName, "Name of the ONNX model file")
      ->required();
  CLI11_PARSE(CLIApp, argc, argv);

  GiantGraph::GraphBuilder GBuilder{ModelFileName};
  GiantGraph::GiantGraph Graph = GBuilder.buildGraph();
  GraphVizardry::dumpGraphViz(Graph);

  auto MLIRModule = MLIRBuild::buildModule(Graph);
  llvm::LLVMContext LLVMContext;
  auto llvmModule = mlir::translateModuleToLLVMIR(MLIRModule, LLVMContext);

  return 0;
}
