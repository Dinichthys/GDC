#include "Test.hpp"
#include "OpTests.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#define CATCH_CONFIG_MAIN

#include "graph/Graph.hpp"
#include "graph/GraphBuilder.hpp"

namespace Tests {

#define OP_TEST(op)                                                            \
  TEST_CASE(#op " node case") {                                                \
    GiantGraph::GraphBuilder GBuilder{MAP_OP_CONTEXT.at(op).FileName};         \
    try {                                                                      \
      GiantGraph::GiantGraph Graph = GBuilder.buildGraph();                    \
      REQUIRE(Graph.nodes().size() == 1);                                      \
      REQUIRE(Graph.nodes()[0]->getName().compare(                             \
                  MAP_OP_CONTEXT.at(op).OpName) == 0);                         \
      size_t num_operands = Graph.nodes()[0]->getNumOperands();                \
      size_t num_res = Graph.nodes()[0]->getNumResults();                      \
      REQUIRE(((num_operands >= MAP_OP_CONTEXT.at(op).InputMinNum) &&           \
              (num_operands <= MAP_OP_CONTEXT.at(op).InputMaxNum)));            \
      REQUIRE(((num_res >= MAP_OP_CONTEXT.at(op).OutputMinNum) &&               \
              (num_res <= MAP_OP_CONTEXT.at(op).OutputMaxNum)));                \
    } catch (...) {                                                            \
      REQUIRE(false);                                                          \
    }                                                                          \
    REQUIRE(true);                                                             \
  }

OP_TEST(CONV);
OP_TEST(GEMM);
OP_TEST(ADD);
OP_TEST(RELU);
OP_TEST(MUL);
OP_TEST(MAT_MUL);
OP_TEST(MAX_POOL);

#undef OP_TEST

}; // namespace Tests
