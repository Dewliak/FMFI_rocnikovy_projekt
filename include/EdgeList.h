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
    std::map<std::pair<int,int>, int>  edge_map;
    std::vector<std::pair<int,int>> edge_list;

    EdgeList(const std::vector<std::vector<int>> &two_dimensional_array);

    //explicit EdgeList(const Graph &graph);
    EdgeList();
    //EdgeList(std::map<);
    std::map<std::pair<int,int>,int> getEdgeMap() const;
    std::vector<std::pair<int,int>> getEdgeList() const;

};

#endif //ROCNIKOVY_PROJEKT_EDGELIST_H
