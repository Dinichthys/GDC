#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"
#include "graph/GraphVizard.hpp"

int main(int argc, char **argv) {
  CLI::App CLIApp;
  std::string ModelFileName;
  CLIApp.add_option("-f,--file", ModelFileName, "Name of the ONNX model file")
      ->required();
  CLI11_PARSE(CLIApp, argc, argv);

  GiantGraph::GraphBuilder GBuilder{ModelFileName};
  GiantGraph::GiantGraph Graph = GBuilder.buildGraph();
  GraphVizardry::dumpGraphViz(Graph);

  return 0;
}
