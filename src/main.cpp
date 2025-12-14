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
#include <cassert>

#include "graph/GraphAlgorithms.h"
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


int hamming_distance(vector<pair<Edge,int>> major_edges, vector<pair<Edge,int>> minor_edges) {
    // the major edges are the constant one, in the model the G', so the graph without the defect
    //cout << "Calculating hamming" << endl;
    unordered_map<Edge,int> colors = {};
    int distance = 0;
    for (pair<Edge,int> major_edge: major_edges) {
        colors[major_edge.first] = major_edge.second;
    }

    for (pair<Edge,int> minor_edge: minor_edges) {
        if (!colors.contains(minor_edge.first)) {
            distance++;
            continue;ľ+š
        }

        if (colors[minor_edge.first] != minor_edge.second) {
            distance++;
        }
    }

    return distance;
}



void func(string graph6format, int vertex1, int vertex2) {
    // func
    // check if graph_has edge uv

    int min_h_distance = 10e6;
    vector<pair<Edge,int>> minEdges = {};

    AdjacencyListGraph graph(graph6format);

    if (!graph.containsEdge(Edge(vertex1,vertex2))) {
        throw runtime_error("No such edge");
    }

    if (!graph.containsVertex(vertex1) || !graph.containsVertex(vertex2)) {
        throw runtime_error("No vertex1 or vertex2");
    }

    // Create G' = G - {vertex1,vertex2}

    AdjacencyListGraph G(graph6format); // G'

    // remove all edges of vertex1 and vertex2 in


     for (Edge e: G.getNeighborEdges(vertex1)){
         G.removeEdge(e);
     }

    for (Edge e: G.getNeighborEdges(vertex2)) {
        G.removeEdge(e);
    }

    //assert

    // Find coloring G' => EdgeList
    ColoringSAT myColoringSAT(G,3);

    vector<Edge> edge_list =  G.getEdgeList().getEdgeList();

    myColoringSAT.encodeConstraints();
    std::cerr << "SAT constructed" << std::endl;

    vector<pair<Edge,int>> edge_list_color = {};
    if (myColoringSAT.solve()) {
        //std::cerr << "SAT solved" << std::endl;
        int index = 0;
        for (int i: myColoringSAT.getColoring()) {
            edge_list_color.push_back(std::make_pair(edge_list.at(index), i));
            index++;
        }
    }
    else {
        std::cerr<< "SAT failed" << std::endl;
        assert(false); // shouldnt fail the sat
    }

    // Find every cooring with defect 3
    static int DEFECT = 3;
    for (vector<Edge> defected_edges: GraphAlgorithms::getGraphsWithoutKEdges(graph,DEFECT)) {
        AdjacencyListGraph defected_graph(defected_edges);

        ColoringSAT defected_coloring(defected_graph,3);
        defected_coloring.encodeConstraints();

        defected_coloring.solve();

        // make it ALLSAT;

        vector<pair<Edge,int>> defected_edge_list_color = {};
        vector<Edge> defected_edge_list =  defected_graph.getEdgeList().getEdgeList();

        int col_counter = 0;
        for (const vector<int>& def_col : defected_coloring.getAllColoring()) {
            defected_edge_list_color.clear();
            int index = 0;
            for (int i: def_col) {
                defected_edge_list_color.emplace_back(defected_edge_list.at(index), i);
                index++;
            }
            //cout << "col_counter: " << col_counter << endl;
            col_counter++;
            int h_distance = hamming_distance(edge_list_color,defected_edge_list_color);
            // find minimum differnece - Hamming distance

            if (h_distance < min_h_distance) {

                min_h_distance = h_distance;

                minEdges = defected_edge_list_color;


            }
        }
        /*
        if (defected_coloring.solve()) {
            //std::cerr << "SAT solved" << std::endl;
            int index = 0;
            for (int i: defected_coloring.getColoring()) {
                defected_edge_list_color.push_back(std::make_pair(defected_edge_list.at(index), i));
                index++;
            }


            int h_distance = hamming_distance(edge_list_color,defected_edge_list_color);
            // find minimum differnece - Hamming distance

            if (h_distance < min_h_distance) {

                min_h_distance = h_distance;

                minEdges = defected_edge_list_color;


            }


        }
        */



    }



    cout << "RESULTS:" << endl;

    cout << "ORIGINAL GRAPH: " << endl;
    graph.printGraph();
    cout << "========================" << endl;
    cout << "CLOSEST COLORING: " << min_h_distance << " " << minEdges.size() <<  endl;


    for (pair<Edge,int> edge: minEdges) {
        cout << "[ " << edge.first.getFirst() << ", " << edge.first.getSecond() << " ] - "  << edge.second << endl;
    }


}




int main() {

    string format = "C~";
    //AdjacencyListGraph graph(format);
    //AdjacencyListGraph graph;
    //"/home/dewliak/CLionProjects/rocnikovy_projekt/data/4_edge_critical_snarks.10.g6" // old full path
    std::vector<std::string> filenames = {"../data/4_edge_critical_snarks.18.g6"};
    std::cout << "Working dir: "
             << std::filesystem::current_path() << "\n";
    for (std::string filename : filenames) {
        std::ifstream file(filename);

        cout << "file is opened: " << filename << endl;
        string s;
        int graph_counter = 1;
        while (getline(file, s)) {
            std::cout << "Graph #" << graph_counter << "  loaded " << std::endl;

            func(s,0,4);

            /*
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
            */
        }

        file.close();


    }

    return 0;
}
