#include "cadical.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <filesystem>
#include <fstream>

#include "graph/AdjacencyListGraph.h"
#include "sat/ColoringSAT.h"
#include <cassert>
#include <ranges>

#include "graph/GraphAlgorithms.h"


#include "defect_search/structures.h"
#include "defect_search/assignmentGeneration.h"
#include "defect_search/defectComputing.h"
#include "defect_search/utilities.h"

#include "export/exportPython.h"
#include "sat/DefectSAT.h"

using namespace std;




vector<Solution> solutions;


void printMatchings(const std::set<Edge>& M1,
                    const std::set<Edge>& M2,
                    const std::set<Edge>& M3)
{
    std::cout << "Matching M1:\n";
    for(const Edge& e: M1) {
        std::cout << "(" << e.getFirst() << "," << e.getSecond() << ") ";
    }
    std::cout << "\n";

    std::cout << "Matching M2:\n";
    for(const Edge& e: M2) {
        std::cout << "(" << e.getFirst() << "," << e.getSecond() << ") ";
    }
    std::cout << "\n";

    std::cout << "Matching M3:\n";
    for(const Edge& e: M3) {
        std::cout << "(" << e.getFirst() << "," << e.getSecond() << ") ";
    }
    std::cout << "\n";
}

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
            continue;
        }

        if (colors[minor_edge.first] != minor_edge.second) {
            distance++;
        }
    }

    return distance;
}

int hammingDistanceForDefect(Solution original, Solution defect) {
    auto allEdges = [](const Solution& sol) {
        std::set<Edge> edges;
        for (const Edge& e : sol.M1) edges.insert(e);
        for (const Edge& e : sol.M2) edges.insert(e);
        for (const Edge& e : sol.M3) edges.insert(e);
        return edges;
    };

    int distance = 0;

    set<Edge> originalEdges = allEdges(original);

    for (const Edge& e: originalEdges) {

        if (original.M1.contains(e) !=  defect.M1.contains(e)) {
            distance++;
        }

        if (original.M2.contains(e) !=  defect.M2.contains(e)) {
            distance++;
        }

        if (original.M3.contains(e) !=  defect.M3.contains(e)) {
            distance++;
        }

    }

    return distance;
}


void func(string graph6format, int vertex1, int vertex2) {

    //int min_h_distance = 10e6;
    vector<pair<Edge,int>> minEdges = {};

    AdjacencyListGraph original_graph(graph6format);

    vector<int> neighbours_v1 = {}; // neighbours of vertex1
    vector<int> neighbours_v2 = {}; // neighbours of vertex2

    /**
         * 1. Check constrains
         *
         *  does graph has edge u-v
    */

    EdgeList og_edge_list = original_graph.getEdgeList();


    if (!original_graph.containsEdge(Edge(vertex1,vertex2))) {
        throw runtime_error("No such edge");
    }

    if (!original_graph.containsVertex(vertex1) || !original_graph.containsVertex(vertex2)) {
        throw runtime_error("No vertex1 or vertex2");
    }

    /**
         * 2. Remove vertices u,v and all of its neighborous edges
         *  since we are adjusting the original(we dont make a new copy) G will remain that new graph
    */

    // Create G' = G - {vertex1,vertex2}

    AdjacencyListGraph G(graph6format); // G'

    // remove all edges of vertex1 and vertex2 in

    // store neightbours
    for (int neighbour: G.getNeighborVertices(vertex1)) {
        if (neighbour == vertex2) continue;
        neighbours_v1.push_back(neighbour);
    }

    for (int neighbour: G.getNeighborVertices(vertex2)) {
        if (neighbour == vertex1) continue;
        neighbours_v2.push_back(neighbour);
    }

    vector<Edge> deleted_edges = {};
    for (Edge e: G.getNeighborEdges(vertex1)){


         deleted_edges.push_back(e); // save edges so we can restore later
         G.removeEdge(e);
     }

    for (Edge e: G.getNeighborEdges(vertex2)) {
        deleted_edges.push_back(e); // save edges so we can restore later
        G.removeEdge(e);
    }

    //G.removeVertex(vertex1);
    //G.removeVertex(vertex2);



    /**
         * 3. Find coloring for G
         *  since it was a critical snark it should have, if not throw error
    */

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

    set<Edge> M1 = {}, M2 = {}, M3 = {}; // create matchings sets
    map<int,int> baseline;
    for (int i = 0; i < edge_list_color.size(); i++) {
        baseline[i] = edge_list_color[i].second;
    }

    for (pair<Edge,int> edge_pair: edge_list_color) {
        switch (edge_pair.second) {
            case 0:
                M1.insert(edge_pair.first);
                break;
            case 1:
                M2.insert(edge_pair.first);
                break;
            case 2:
                M3.insert(edge_pair.first);
                break;
            default: ;
        }
    }



    cout << M1.size() <<  " " << M2.size() << " " << M3.size() << endl;


    /**
         * 4. Readd the vertices u,v
         *
         */


    Solution original_solution;

    original_solution.M1 = M1;
    original_solution.M2 = M2;
    original_solution.M3 = M3;

    exportPython(original_solution, "../export_data/original_sol.txt", og_edge_list.edge_list, true);




    /**
         * 4. Readd the vertices u,v
         *
         */


    EdgeList modifiedGraphEdgeList = G.getEdgeList();

    for (Edge edge: deleted_edges) {
        modifiedGraphEdgeList.addEdge(edge); // we put the deleted edges back to the edge list, this way it stil be compatible
        // but it might not be if we used the original graph and it's own edgelsit, since the deleted
        // edges fro mthe edges might've been in the middle
    }

    // iterative deepening

    /*
    for (int k = 0; k <= (int)og_edge_list.size(); k++) {
        DefectSAT defectSAT(original_graph, modifiedGraphEdgeList, baseline);  // fresh solver each k
        if (defectSAT.solveAtDistance(k)) {
            cout << "Solved at distance: " << k << endl;
            Solution r = defectSAT.extractSolution();
            printMatchings(r.M1,r.M2,r.M3);
            cout << "CALCULATED HAMMING: " << hammingDistanceForDefect(original_solution,r) << endl;
            exportPython(r, "../export_data/sol.txt", og_edge_list.edge_list, true);
            break;
        }
        std::cout << "No solution at Hamming distance " << k << "\n";
    }
    */
    DefectSAT defectSAT(original_graph, modifiedGraphEdgeList, baseline);  // fresh solver each k
    vector<Solution> allSol = defectSAT.getAllSolutions();

    int minHammingDistance = hammingDistanceForDefect(original_solution,allSol[0]);
    Solution minSolution = allSol[0];

    for (Solution solution: allSol) {
        int hammingDistance = hammingDistanceForDefect(original_solution,solution);
        if (hammingDistance < minHammingDistance) {
            minHammingDistance = hammingDistance;
            minSolution = solution;
        }
    }
    cout << "Amount of solutions: " << allSol.size() << endl;
    cout << "Calculated min. hammign distance: " << minHammingDistance << endl;
    exportPython(minSolution, "../export_data/sol.txt", og_edge_list.edge_list, true);
}

///// SO FAR SEEMS GOOD, JUST PROBLEM WITH THE HAMMING DISTANCE, FIRST TRY MIGHT BE BETTER
//// TO BRUTE FORCE ALL SOLUTIONS AND JUST CHECK HAMMING DISTANCE MANUALLYx

#


#define DEBUG_SINGLE_GRAPH 1

int main() {

#if DEBUG_SINGLE_GRAPH

    std::string s = "Q?hY@eOGG??B_??@g???T?a??@g";   // your test graph6 string
    std::cout << "Running single test graph\n";
    func(s, 1, 5);

#else

    string format = "C~";

    std::vector<std::string> filenames = {"../data/4_edge_critical_snarks.18.g6"};

    std::cout << "Working dir: "
              << std::filesystem::current_path() << "\n";

    for (std::string filename : filenames) {

        std::ifstream file(filename);

        std::cout << "file is opened: " << filename << std::endl;

        string s;
        int graph_counter = 1;

        while (getline(file, s)) {

            std::cout << "Graph #" << graph_counter << " loaded" << std::endl;

            func(s,0,4);

            graph_counter++;
        }

        file.close();
    }

#endif

}