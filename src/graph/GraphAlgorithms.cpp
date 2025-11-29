//
// Created by dewliak on 10/29/25.
//

#include <algorithm>
#include <bitset>
#include <graph/IGraph.h>
#include <queue>
#include <map>
#include <stdexcept>
#include <generator>
namespace GraphAlgorithms {

    int shortest_cycle(const IGraph& graph) {

        std::vector<int> vertices = graph.getVertices();

        if (vertices.empty()) {
            return 0;
        }

        std::map<int,int> parent;
        std::map<int,int> distance;
        std::queue<int> q;

        int MAX_INT = (int)10e9;

        for (int v: vertices) {
            parent[v] = -1;
            distance[v] = MAX_INT;
        }


        // source is the first node;
        // we assume the graph is not a forest
        // because of the check earlier we know vertices.size() >= 1
        q.push(vertices[0]);

        int answer = MAX_INT;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int child: graph.getNeighborVertices(node)) {
                if (distance[child] == MAX_INT) {
                    distance[child] = distance[node] + 1;
                    parent[child] = node;
                    q.push(child);
                }
                else if (parent[node] != child &&  parent[child] != node) {
                    answer = std::min(answer, distance[child] + distance[node] + 1);
                }


            }

        }

        return answer;

    }


    Matrix graph6format_to_matrix(std::string graph6format) {
        int vertices = static_cast<int>(graph6format[0]) - 63; // (char)format[0] casts it to a signed char, we need unsigned

        string all_bits = "";
        for (int i = 1; i < graph6format.size(); i++) {
            int value = static_cast<int>(graph6format[i]) - 63;
            std::bitset<6> bits(value);
            all_bits += bits.to_string();
        }


        Matrix matrix = Matrix(vertices,vertices);

        //matrix = vector(vertices,vector<int>(vertices,0));

        int num_in_bot_left_diag = 0;
        for (int i = 0; i < vertices; i++) {
            num_in_bot_left_diag += i;
        }

        string bot_left_diag = all_bits.substr(0, num_in_bot_left_diag);

        size_t pos = 0;
        for (int i =0; i < vertices; i++) {
            // fill the first i with zeroes, cuz the triangle but the constructor already fills it up

            for (int j = 0; j < i; j++) {
                matrix.set(i,j,all_bits[pos++] - '0');
                //matrix[i][j] = all_bits[pos++] - '0';
            }

        }
        // add transpose
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < i; j++) {
                matrix.set(j,i, matrix(i,j));
                //            matrix[j][i] = matrix[i][j];
            }
        }

        return matrix;

    }

    set<vector<Edge>> getGraphsWithoutKEdges(const IGraph& graph, int k ) {



        vector<Edge> edge_list = graph.getEdgeList().edge_list;

        if (k > edge_list.size()) {
            throw  std::runtime_error("k (the number of edges to exclude), has to be smaller then the number of edges");
        }

        vector<bool> edges_included = vector(edge_list.size(), true);
        for (int i =0; i < k;i++) {
            edges_included[i] = false;
        }

        set<vector<Edge>> all_possibilities = {};

        do {
            vector<Edge> included = {};
            for (int i =0; i < edge_list.size(); i++) {
                if (edges_included[i]) {
                    included.push_back(edge_list[i]);
                }
            }

            all_possibilities.insert(included);

        }while (std::next_permutation(edges_included.begin(), edges_included.end()));

        return all_possibilities;
    }

}
