//
// Created by dewliak on 10/29/25.
//

#ifndef ROCNIKOVY_PROJEKT_ADJECENCYLISTGRAPH_H
#define ROCNIKOVY_PROJEKT_ADJECENCYLISTGRAPH_H


#include "Edge.h"
#include "IGraph.h"



class AdjacencyListGraph: public IGraph<int,Edge> {

public:
    // vertex op
    void addVertex(const int& vertex) override;
    void removeVertex(const  int& vertex) override;
    bool containsVertex(const int& vertex) const override;

    //edge op
    void addEdge(const Edge& edge) override;
    void removeEdge(const Edge& edge) override;
    bool containsEdge(const Edge& edge) const override;

    // traverse graph
    std::vector<int> getVertices() const override;
    std::vector<Edge> getEdges() const override;
    std::vector<int> getNeighborVertices(const int& vertex) const override;
    std::vector<Edge> getNeighborEdges(const int& vertex) const override;

    // data
    int getVertexCount() const override;
    int getEdgeCount() const override;
    int getDegree(const int& vertex) const override;

    void printGraph();
private:
        std::map<int,std::vector<Edge>> adjacencyList;
};


#endif //ROCNIKOVY_PROJEKT_ADJECENCYLISTGRAPH_H