//
// Created by dewliak on 10/29/25.
//

#include "../../include/graph/AdjacencyListGraph.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <set>

void AdjacencyListGraph::addVertex(const int &vertex) {
    if (!adjacencyList.contains(vertex)) {
        adjacencyList[vertex] = std::vector<Edge>();
    }
}

void AdjacencyListGraph::removeVertex(const int &vertex) {
    //assert(adjacencyList.contains(vertex));
    //assert(adjacencyList[vertex].empty());

    if (!containsVertex(vertex)) {
        throw std::runtime_error("Vertex doesn't exists");
    }

    if (!adjacencyList[vertex].empty()){
        throw std::runtime_error("Vertex is not empty");
    }

    adjacencyList.erase(vertex);
}

bool AdjacencyListGraph::containsVertex(const int &vertex) const {
    return adjacencyList.contains(vertex);
}

void AdjacencyListGraph::addEdge(const Edge &edge) {
    int f = edge.getFirst();
    int s = edge.getSecond();

    addVertex(f);
    addVertex(s);

    adjacencyList.at(f).push_back(edge);
    adjacencyList.at(s).push_back(edge);

}

void AdjacencyListGraph::removeEdge(const Edge &edge) {
    int f = edge.getFirst();
    int s = edge.getSecond();

    if (!containsVertex(f) || !containsVertex(s)) {
        throw std::runtime_error("One or both vertices do not exist");
    }

    auto ff = std::find(adjacencyList.at(f).begin(), adjacencyList.at(f).end(), edge);

    if (ff != adjacencyList.at(f).end()) {
        adjacencyList.at(f).erase(ff);
    }
    else {
        throw std::runtime_error("Edge not found");
    }

    auto fs = std::find(adjacencyList.at(s).begin(), adjacencyList.at(s).end(), edge);

    if (fs != adjacencyList.at(s).end()) {
        adjacencyList.at(s).erase(fs);
    }
    else {
        throw std::runtime_error("Edge not found");
    }

    if (adjacencyList.at(f).empty()) {
        removeVertex(f);
    }

    if (adjacencyList.at(s).empty()) {
        removeVertex(s);
    }
}

bool AdjacencyListGraph::containsEdge(const Edge &edge) const {

    int f = edge.getFirst();
    int s = edge.getSecond();

    if (!containsVertex(f) || !containsVertex(s)) {
        return false;
    }

    const auto it = std::find(adjacencyList.at(f).begin(), adjacencyList.at(f).end(), edge);
    
    return (it != adjacencyList.at(f).end());
}

std::vector<int> AdjacencyListGraph::getVertices() const {
    std::vector<int> v = {};

    for (const auto& it: adjacencyList) {
        v.push_back(it.first);
    }

    return v;
}

std::vector<Edge> AdjacencyListGraph::getEdges() const {
    std::set<Edge> edges_set = {};

    for (const auto& it: adjacencyList) {
        for (const Edge e: it.second) {
            edges_set.insert(e);
        }
    }

    std::vector<Edge> edges = {};
    edges.assign(edges_set.begin(), edges_set.end());
    return edges;

}

std::vector<int> AdjacencyListGraph::getNeighborVertices(const int &vertex) const{

    assert(containsVertex(vertex));
    std::vector<int> neighbor = {};

    for (const Edge e: adjacencyList.at(vertex)) {
        if (e.getFirst() == vertex) {
            neighbor.push_back(e.getSecond());
        }
        else {
            neighbor.push_back(e.getFirst());
        }
    }

    return neighbor;
}

std::vector<Edge> AdjacencyListGraph::getNeighborEdges(const int& vertex) const {

    assert(containsVertex(vertex));

    std::vector<Edge> neighbor = {};
    neighbor.assign(adjacencyList.at(vertex).begin(),adjacencyList.at(vertex).end());

    return neighbor;
}

EdgeList AdjacencyListGraph::getEdgeList() const {
    std::map<Edge, int>  edge_map = {};
    std::vector<Edge> edge_list = {};

    // invariant [i,j] always i < j

    int counter =0;
    for (const auto& it: adjacencyList) {
        const int first_vertex = it.first;

        for (Edge e: it.second) {
            int other_vertex = ((first_vertex == e.getFirst()) ? e.getSecond() : e.getFirst());

            if (first_vertex < other_vertex) {
                edge_map[e] = counter;
                edge_list.push_back(e);
                counter++;
            }
        }
    }


    return EdgeList(edge_map, edge_list);

}

int AdjacencyListGraph::getVertexCount() const {
    return adjacencyList.size();
}

int AdjacencyListGraph::getEdgeCount() const {
    // TODO: might be buggy, because I assume that there is even number of edges, that should be

    int amount = 0;
    for (const auto& it: adjacencyList) {
        amount += it.second.size();
    }

    return  (int)amount/2;
}

int AdjacencyListGraph::getDegree(const int &vertex) const {
    assert(containsVertex(vertex));

    return adjacencyList.at(vertex).size();
}


void AdjacencyListGraph::printGraph() {
    for (const auto& it: adjacencyList) {
        std::cout << "[ " << it.first << " ]:" << std::endl;;
        for (const Edge e: it.second) {
            std::cout << e.getFirst() << "->" << e.getSecond() << std::endl;
        }
        std::cout << "--------------------" << std::endl;
    }
}


