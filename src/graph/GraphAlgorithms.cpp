//
// Created by dewliak on 10/29/25.
//

#include <../include/graph/IGraph.h>
#include <queue>
#include <map>

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
}
