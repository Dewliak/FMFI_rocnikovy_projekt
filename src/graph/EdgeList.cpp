
// Created by dewliak on 10/22/25.
//


#include "../../include/graph/EdgeList.h"

EdgeList::EdgeList(const std::vector<std::vector<int>> &two_dimensional_array) {
    edge_list = {};
    edge_map = std::map<Edge,int>();

    int counter = 0;
    for (int i =0; i < two_dimensional_array.size(); i++) {
        for (int j =i+1; j < two_dimensional_array[i].size();j++) {
            if (two_dimensional_array[i][j] != 0) {
                Edge edge = Edge(i,j);
                edge_map[edge] = counter;
                edge_list.push_back(edge);
                counter++;
            }
        }
    }
}

EdgeList::EdgeList(std::map<Edge, int> graph_edge_map, std::vector<Edge> graph_edge_list) {
    edge_list = graph_edge_list;
    edge_map = graph_edge_map;
}

/*
EdgeList::EdgeList(const Graph &graph) {
    edge_list = {};
    edge_map = {};
    std::vector<std::vector<int>> two_dimensional_array = graph.getMatrix();
    int counter = 0;
    for (int i =0; i < two_dimensional_array.size(); i++) {
        for (int j =i+1; j < two_dimensional_array[i].size();j++) {
            if (two_dimensional_array[i][j] != 0) {
                pair<int,int> edge = make_pair(i,j);
                edge_map[edge] = counter;
                edge_list.push_back(edge);
                counter++;
            }
        }
    }
}
*/

std::map<Edge, int> EdgeList::getEdgeMap() const {
    return edge_map;
}

std::vector<Edge> EdgeList::getEdgeList() const {
    return edge_list;
}