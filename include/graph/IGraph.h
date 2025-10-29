//
// Created by dewliak on 10/29/25.
//

#ifndef ROCNIKOintY_PROJEKT_IGRAPH_H
#define ROCNIKOintY_PROJEKT_IGRAPH_H


#include <vector>
#include <../include/graph/EdgeList.h>
#include <../include/graph/Edge.h>

class IGraph {
    public:
        virtual ~IGraph() = default;

        // vertex op
        virtual void addVertex(const int& vertex) =0;
        virtual void removeVertex(const int& vertex) = 0;
        virtual bool containsVertex(const int& vertex) const = 0;

        //edge op
        virtual void addEdge(const Edge& edge) =0;
        virtual void removeEdge(const Edge& edge) =0;
        virtual bool containsEdge(const Edge& edge) const = 0;

        // traverse graph
        virtual std::vector<int> getVertices() const = 0;
        virtual std::vector<Edge> getEdges() const = 0;
        virtual std::vector<int> getNeighborVertices(const int& vertex) const = 0;
        virtual std::vector<Edge> getNeighborEdges(const int& vertex) const =0;
        virtual EdgeList getEdgeList() const = 0;
        // data
        virtual int getVertexCount() const = 0;
        virtual int getEdgeCount() const = 0;
        virtual int getDegree(const int& vertex) const =0;


};

#endif //ROCNIKOintY_PROJEKT_IGRAPH_H