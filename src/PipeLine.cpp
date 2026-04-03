//
// Created by dewliak on 4/2/26.
//

#include <cassert>
#include <iostream>

#include "Pipeline.h"
#include "defect_search/utilities.h"
#include "graph/AdjacencyListGraph.h"
#include "ilp/DefectILP.h"
#include "sat/ColoringSAT.h"
#include "sat/DefectSAT.h"


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


GraphColoringData generateColoring(string graph6format, int vertex1, int vertex2) {
        //int min_h_distance = 10e6;

    AdjacencyListGraph original_graph(graph6format);

    vector<int> neighbours_v1 = {}; // neighbours of vertex1
    vector<int> neighbours_v2 = {}; // neighbours of vertex2

    /**
         * 1. Check constrains
         *
         *  does graph has edge u-v
    */

    EdgeList originalGraphEdgeList = original_graph.getEdgeList();


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


    Solution originalSolution;

    originalSolution.M1 = M1;
    originalSolution.M2 = M2;
    originalSolution.M3 = M3;

    //exportPython(originalSolution, "../export_data/original_sol.txt", originalGraphEdgeList.edge_list, true);


    EdgeList modifiedGraphEdgeList = G.getEdgeList();


    /**
     * 4. Readd the vertices u,v
     *
     */

    for (Edge edge: deleted_edges) {
        modifiedGraphEdgeList.addEdge(edge); // we put the deleted edges back to the edge list, this way it stil be compatible
        // but it might not be if we used the original graph and it's own edgelsit, since the deleted
        // edges fro mthe edges might've been in the middle
    }


    GraphColoringData data(
        graph6format,
        baseline,modifiedGraphEdgeList,
        originalSolution,
        originalGraphEdgeList,
        ((vertex1 < vertex2) ? vertex1 : vertex2),
        ((vertex1 > vertex2) ? vertex1 : vertex2));

    return data;

}

// TODO: In someway conenct it
vector<GraphColoringData> generateAllColoring(string graph6format, int vertex1, int vertex2) {
        //int min_h_distance = 10e6;
    vector<GraphColoringData> allData = {};

    AdjacencyListGraph original_graph(graph6format);

    vector<int> neighbours_v1 = {}; // neighbours of vertex1
    vector<int> neighbours_v2 = {}; // neighbours of vertex2

    /**
         * 1. Check constrains
         *
         *  does graph has edge u-v
    */

    EdgeList originalGraphEdgeList = original_graph.getEdgeList();


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

    vector<vector<int>> allColoring = myColoringSAT.getAllColoring();


    for (vector<int> coloring: allColoring) {
        int index = 0;
        edge_list_color.clear();

        for (int i: coloring) {
            edge_list_color.push_back(std::make_pair(edge_list.at(index), i));
            index++;
        }
        set<Edge> M1 = {}, M2 = {}, M3 = {}; // create matchings sets
        map<int,int> baseline ={};
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


        Solution originalSolution;

        originalSolution.M1 = M1;
        originalSolution.M2 = M2;
        originalSolution.M3 = M3;

        //exportPython(originalSolution, "../export_data/original_sol.txt", originalGraphEdgeList.edge_list, true);


        EdgeList modifiedGraphEdgeList = G.getEdgeList();


        /**
         * 4. Readd the vertices u,v
         *
         */

        for (Edge edge: deleted_edges) {
            modifiedGraphEdgeList.addEdge(edge); // we put the deleted edges back to the edge list, this way it stil be compatible
            // but it might not be if we used the original graph and it's own edgelsit, since the deleted
            // edges fro mthe edges might've been in the middle
        }

        allData.emplace_back(graph6format,
            baseline,
            modifiedGraphEdgeList,
            originalSolution,
            originalGraphEdgeList,
            ((vertex1 < vertex2) ? vertex1 : vertex2),
            ((vertex1 > vertex2) ? vertex1 : vertex2));
    }


    return allData;

}


Solution findClosestWithDefectThree(
    const string originalGraphFormat,
    const EdgeList& modifiedGraphEdgeList,
    const Solution& originalSolution,
    const map<int,int>& baseline,
    SearchStrategy strategy) {

    AdjacencyListGraph originalGraph(originalGraphFormat);

    if (strategy == SearchStrategy::BruteForce) {
        DefectSAT defectSAT(originalGraph, modifiedGraphEdgeList, baseline);  // fresh solver each k
        vector<Solution> allSol = defectSAT.getAllSolutions();

        int minHammingDistance = hammingDistanceForDefect(originalSolution,allSol[0]);
        Solution minSolution = allSol[0];

        for (const Solution& solution: allSol) {
            int hammingDistance = hammingDistanceForDefect(originalSolution,solution);
            if (hammingDistance < minHammingDistance) {
                minHammingDistance = hammingDistance;
                minSolution = solution;
            }
        }
        cout << "Amount of solutions: " << allSol.size() << endl;
        cout << "Calculated min. hammign distance: " << minHammingDistance << endl;
        cout << "Amount of edges: " << originalGraph.getVertexCount() << endl;
        printMatchings(minSolution.M1,minSolution.M2,minSolution.M3);
        return minSolution;
        //exportPython(minSolution, "../export_data/sol.txt", og_edge_list.edge_list, true);
    }
    else if (strategy == SearchStrategy::Incremental) {
        DefectSAT defectSAT(originalGraph, modifiedGraphEdgeList, baseline);
        /*
        for (int k = 0; k <= (int)og_edge_list.size(); k++) {
            if (defectSAT.solveAtDistance(k)) {
                Solution r = defectSAT.extractSolution();
                cout << "Solved at Hamming distance: " << k << "\n";
                cout << "Verified Hamming: " << hammingDistanceForDefect(originalSolution, r) << "\n";
                return r;
                //exportPython(r, "../export_data/sol.txt", og_edge_list.edge_list, true);
                break;
            }
            cout << "No solution at distance " << k << "\n";
        }
        */
        return originalSolution; // error
    }
    else if (strategy == SearchStrategy::ILP) {
        DefectILP ilp(originalGraph, modifiedGraphEdgeList, baseline);
        ILPResult r = ilp.solve();

        if (r.found) {
            cout << "ILP Hamming distance: " << r.hammingDistance << "\n";
            cout << "Verified:             "
                 << hammingDistanceForDefect(originalSolution, r.solution) << "\n";

            return r.solution;
            //exportPython(r.solution, "../export_data/sol.txt",
                        // og_edge_list.edge_list, true);
        }
    }

    else{
        throw runtime_error("Invalid strategy in func()");
    }

}
