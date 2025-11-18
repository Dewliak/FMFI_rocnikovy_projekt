//
// Created by dewliak on 10/22/25.
//

#ifndef ROCNIKOVY_PROJEKT_EDGELIST_H
#define ROCNIKOVY_PROJEKT_EDGELIST_H
#include <map>
#include <utility>

#include "Graph.h"
#include "Edge.h"

struct EdgeList {
    std::map<Edge, int>  edge_map;
    std::vector<Edge> edge_list;

    EdgeList(const std::vector<std::vector<int>> &two_dimensional_array);

    //explicit EdgeList(const Graph &graph);
    EdgeList(std::map<Edge, int>  graph_edge_map, std::vector<Edge> graph_edge_list);
    //EdgeList(std::map<);
    std::map<Edge, int> getEdgeMap() const;
    std::vector<Edge> getEdgeList() const;

};

#endif //ROCNIKOVY_PROJEKT_EDGELIST_H
