//
// Created by dewliak on 10/29/25.
//

#ifndef ROCNIKOVY_PROJEKT_IGRAPH_H
#define ROCNIKOVY_PROJEKT_IGRAPH_H

#include <map>
#include <vector>
#include <EdgeList.h>

template<typename V, typename E>
class IGraph {
    public:
        virtual ~IGraph() = default;

        // vertex op
        virtual void addVertex(const V& vertex) =0;
        virtual void removeVertex(const V& vertex) = 0;
        virtual bool containsVertex(const V& vertex) const = 0;

        //edge op
        virtual void addEdge(const E& edge) =0;
        virtual void removeEdge(const E& edge) =0;
        virtual bool containsEdge(const E& edge) const = 0;

        // traverse graph
        virtual std::vector<V> getVertices() const = 0;
        virtual std::vector<E> getEdges() const = 0;
        virtual std::vector<V> getNeighborVertices(const V& vertex) const = 0;
        virtual std::vector<E> getNeighborEdges(const V& vertex) const =0;
        virtual EdgeList getEdgeList() const = 0;
        // data
        virtual int getVertexCount() const = 0;
        virtual int getEdgeCount() const = 0;
        virtual int getDegree(const V& vertex) const =0;


};

#endif //ROCNIKOVY_PROJEKT_IGRAPH_H