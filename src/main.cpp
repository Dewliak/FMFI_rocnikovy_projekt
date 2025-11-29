#include "cadical.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <filesystem>
#include <fstream>

#include "graph/AdjacencyListGraph.h"
#include "graph/Graph.h"
#include "sat/ColoringSAT.h"
using namespace std;
/*
def graph6ToAdj(graph6):
    # vertices + 63 = first char
    vertices = ord(graph6[0]) - 63

    bin_list = ""

    # Turn into 6 bit pieces
    for i in graph6[1:]:
        bin_list += ("{0:06b}".format(ord(i) - 63))

    adjMatrix = []

    # Unpad on right until have bottom left diag
    num_in_bot_left_diag = 0
    for i in range(vertices):
        num_in_bot_left_diag += i

    bot_left_diag = bin_list[:num_in_bot_left_diag]

    for i in range(0, vertices):
        sub_adjMatrix = [0 for i in range(vertices)]
        for j in range(i):
            sub_adjMatrix[j] = int(bin_list[0])
            bin_list = bin_list[1:]
        adjMatrix.append(sub_adjMatrix)

    addTranspose(adjMatrix)

    return vertices, adjMatrix




    // Variables are represented by positive integers: 1, 2, 3...
    // Negative integers represent negation
    // Example: (x1 ∨ x2) ∧ (¬x1 ∨ x2)

    // Clause 1: x1 ∨ x2
    solver.add(1);
    solver.add(2);
    solver.add(0); // 0 terminates the clause

    // Clause 2: ¬x1 ∨ x2
    solver.add(-1);
    solver.add(2);
    solver.add(0);

    // Solve
    bool sat = solver.solve();

    std::cout << (sat ? "SATISFIABLE" : "UNSATISFIABLE") << std::endl;

    // If SAT, get assignments
    if (sat) {
        std::cout << "x1 = " << solver.val(1)
                  << ", x2 = " << solver.val(2) << std::endl;
    }

 */





int main() {

    string format = "C~";
    //AdjacencyListGraph graph(format);
    //AdjacencyListGraph graph;

    std::vector<std::string> filenames = {"/home/dewliak/CLionProjects/rocnikovy_projekt/data/4_edge_critical_snarks.10.g6"};
    std::cout << "Working dir: "
             << std::filesystem::current_path() << "\n";
    for (std::string filename : filenames) {
        std::ifstream file(filename);

        cout << "file is opened: " << filename << endl;
        string s;
        int graph_counter = 1;
        while (getline(file, s)) {
            std::cout << "Graph #" << graph_counter << "  loaded " << std::endl;



            AdjacencyListGraph  myGraph(s);
            myGraph.printGraph();
            std::cout << "Graph #" << graph_counter << "  constructed " << std::endl;

            myGraph.removeEdge(Edge(0,4));
            myGraph.removeEdge(Edge(0,6));
            myGraph.removeEdge(Edge(0,8));

            myGraph.removeEdge(Edge(4,2));
            myGraph.removeEdge(Edge(4,5));

            //myGraph.removeVertex(0);
            //myGraph.removeVertex(4);

            ColoringSAT myColoringSAT(myGraph,3);

            vector<Edge> edge_list =  myGraph.getEdgeList().getEdgeList();
            std::cout << "SAT constructed" << std::endl;
            myColoringSAT.encodeConstraints();
            if (myColoringSAT.solve()) {
                std::cout << "SAT solved" << std::endl;
                int index = 0;
                for (int i: myColoringSAT.getColoring()) {
                    cout << "[ " << edge_list.at(index).getFirst() << " - " <<  edge_list.at(index).getSecond() << " ] : " << i << endl;
                    index++;
                }


            }
            else {
                std::cout << "SAT failed" << std::endl;
            }
            graph_counter++;
            //EdgeList

        }

        file.close();


    }

    return 0;
}
