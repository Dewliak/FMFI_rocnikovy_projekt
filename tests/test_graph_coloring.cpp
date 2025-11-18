//
// Created by dewliak on 10/7/25.
//
#include <gtest/gtest.h>
#include "Matrix.h"
#include "graph/Graph.h"
#include "graph/AdjacencyListGraph.h"
#include "sat/ColoringSAT.h"
#include <vector>
TEST(GraphColoring, Colorable1) {
    /*
    std::vector<std::vector<int>> three_colorable = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };
    Matrix m(three_colorable);
    Graph g(m);
    */
    string format = "Cl";
    AdjacencyListGraph g(format);
    ColoringSAT s(g,3);
    EXPECT_EQ(s.solve(),true);
}


TEST(GraphColoring, NotColorable1) {
    /*
    std::vector<std::vector<int>> not_three_colorable = {
        {0, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 0, 1},
        {1, 1, 1, 0}
    };
    Matrix m(not_three_colorable);
    Graph g(m);
    */
    string format = "J?@~?@?@"; // Petersen-graph
    AdjacencyListGraph g(format);

    ColoringSAT s(g,3);

    EXPECT_EQ(s.solve(), false);
}

