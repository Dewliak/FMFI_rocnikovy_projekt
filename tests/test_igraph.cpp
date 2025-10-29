//
// Created by dewliak on 10/29/25.
//
// test_adjacencylistgraph.cpp
#include <gtest/gtest.h>
#include <algorithm>
#include "../include/AdjacencyListGraph.h"
#include "../include/IGraph.h"


class GraphTest : public ::testing::Test {
protected:
    AdjacencyListGraph graph;

    void SetUp() override {
        // Initialize graph with some vertices and edges
        graph.addVertex(1);
        graph.addVertex(2);
        graph.addVertex(3);


        graph.addEdge(Edge(1, 2));
        graph.addEdge(Edge(2, 3));
    }

    void TearDown() override {
        // Optional cleanup
    }
};

// ----- Vertex Tests -----
TEST_F(GraphTest, AddVertex) {
    graph.addVertex(4);
    EXPECT_TRUE(graph.containsVertex(4));
}

TEST_F(GraphTest, RemoveVertex) {
    graph.addVertex(4);
    graph.removeVertex(4);
    EXPECT_FALSE(graph.containsVertex(4));
}

TEST_F(GraphTest, CantRemoveNonEmptyVertex) {
    EXPECT_THROW(graph.removeVertex(3), std::runtime_error);
    EXPECT_TRUE(graph.containsVertex(3));
}

TEST_F(GraphTest, ContainsVertex) {
    EXPECT_TRUE(graph.containsVertex(1));
    EXPECT_FALSE(graph.containsVertex(10));
}

// ----- Edge Tests -----
TEST_F(GraphTest, AddEdge) {
    Edge e(1, 3);
    graph.addEdge(e);
    EXPECT_TRUE(graph.containsEdge(e));
}

TEST_F(GraphTest, RemoveEdge) {
    Edge e(1, 2);
    EXPECT_TRUE(graph.containsEdge(e));
    graph.removeEdge(e);
    EXPECT_FALSE(graph.containsEdge(e));
}

TEST_F(GraphTest, ContainsEdge) {
    Edge e1(1, 2);
    Edge e2(3, 4);

    EXPECT_TRUE(graph.containsEdge(e1));
    EXPECT_FALSE(graph.containsEdge(e2));
}

// ----- Traversal Tests -----
TEST_F(GraphTest, GetVertices) {
    auto vertices = graph.getVertices();
    EXPECT_EQ(vertices.size(), 3);
    EXPECT_NE(std::find(vertices.begin(), vertices.end(), 1), vertices.end());
    EXPECT_NE(std::find(vertices.begin(), vertices.end(), 2), vertices.end());
    EXPECT_NE(std::find(vertices.begin(), vertices.end(), 3), vertices.end());
}

TEST_F(GraphTest, GetEdges) {
    auto edges = graph.getEdges();
    EXPECT_EQ(edges.size(), 2);
    EXPECT_TRUE(graph.containsEdge(edges[0]));
    EXPECT_TRUE(graph.containsEdge(edges[1]));
}

// ----- Neighbors and Degrees -----
TEST_F(GraphTest, GetNeighborVertices) {
    auto neighbors = graph.getNeighborVertices(2);
    EXPECT_EQ(neighbors.size(), 2);
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), 1), neighbors.end());
    EXPECT_NE(std::find(neighbors.begin(), neighbors.end(), 3), neighbors.end());
}

TEST_F(GraphTest, GetDegree) {
    EXPECT_EQ(graph.getDegree(1), 1);
    EXPECT_EQ(graph.getDegree(2), 2);
    EXPECT_EQ(graph.getDegree(3), 1);
}

// ----- Edge Case Tests -----
TEST_F(GraphTest, RemoveNonexistentEdge) {
    Edge e(10, 20);
    EXPECT_THROW(graph.removeEdge(e), std::runtime_error);
    EXPECT_FALSE(graph.containsEdge(e));
}

TEST_F(GraphTest, RemoveNonexistentVertex) {
    EXPECT_FALSE(graph.containsVertex(10));
    EXPECT_THROW(graph.removeVertex(10), std::runtime_error);
    EXPECT_FALSE(graph.containsVertex(10));
}

// ----- Main function -----
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
