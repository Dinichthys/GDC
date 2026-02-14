#include <CLI/App.hpp>
#include <onnx.pb.h>

#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"


int main(int argc, char **argv) {
  CLI::App CLIApp;
  std::string ModelFileName;
  CLIApp.add_option("-f,--file", ModelFileName, "Name of the ONNX model file")
      ->required();
  CLI11_PARSE(CLIApp, argc, argv);

  GiantGraph::GiantGraph Graph;
  GiantGraph::GraphBuilder GBuilder{ModelFileName, Graph};

  return 0;
}
