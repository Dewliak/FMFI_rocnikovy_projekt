#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <filesystem>
#include <fstream>
#include <ranges>


#include "graph/GraphAlgorithms.h"
#include "defect_search/structures.h"
#include "export/exportPython.h"
#include "sat/DefectSAT.h"

#include "Pipeline.h"
#include "defect_search/utilities.h"

using namespace std;


void exportData(
    const GraphColoringData &coloringData,
    const Solution &defectSolution,
    SearchStrategy search_strategy,
    std::ostream &outStream = std::cout) {
    // we have graph6 format

    string pm1Encoding, pm2Encoding, pm3Encoding, originalColoring;
    string strategy;


    switch (search_strategy) {
        case SearchStrategy::BruteForce:
            strategy = "BF";
            break;
        case SearchStrategy::ILP:
            strategy = "ILP";
            break;
        case SearchStrategy::Incremental:
            strategy = "INC";
            break;
        default:
            strategy = "UNK";
            break;
    }
    int hammingDistance = hammingDistanceForDefect(coloringData.originalSolution, defectSolution);
    // get cannonical form, tj. incremental ordering of edges
    vector<Edge> edges = coloringData.originalGraphEdgeList.getEdgeList();
    sort(edges.begin(), edges.end());

    pm1Encoding.reserve(edges.size());
    pm2Encoding.reserve(edges.size());
    pm3Encoding.reserve(edges.size());
    originalColoring.reserve(edges.size());

    for (const Edge &edge: edges) {
        //cout << "DEBUG: (" << edge.getFirst() << ", " << edge.getSecond() << ")";

        pm1Encoding += defectSolution.M1.contains(edge) ? "1" : "0";
        pm2Encoding += defectSolution.M2.contains(edge) ? "1" : "0";
        pm3Encoding += defectSolution.M3.contains(edge) ? "1" : "0";


        // it is guranteed that edge is exactly in on matching, or none since the grapg (G - {u,v}) is 3-edge colorable


        if (coloringData.originalSolution.M1.contains(edge)) {
            originalColoring += "1";
            cout << " 1 ";
        } else if (coloringData.originalSolution.M2.contains(edge)) {
            originalColoring += "2";
            cout << " 2 ";
        } else if (coloringData.originalSolution.M3.contains(edge)) {
            originalColoring += "3";
            cout << " 3 ";
        } else {
            originalColoring += "0";
            cout << " 4 ";
        }

        //cout << (defectSolution.M1.contains(edge) ? "1" : "0") << " - ";
        //cout << (defectSolution.M2.contains(edge) ? "1" : "0") << " - ";
        //cout << (defectSolution.M3.contains(edge) ? "1" : "0") << endl;
    }

    outStream << coloringData.originalGraphFormat << ","
            << strategy << ","
            << hammingDistance << ","
            << coloringData.vertex1 << ","
            << coloringData.vertex2 << ","
            << originalColoring << ","
            << pm1Encoding << ","
            << pm2Encoding << ","
            << pm3Encoding << "\n";
}

#define DEBUG_SINGLE_GRAPH 0
#define CALC_BRUTEFORCE 0
#define CALC_ILP 1
#define ALL_COLORS 1
#define ALL_PAIRS 1

int main() {
    std::ofstream outFile;
    outFile.open("../export_data/export.csv", std::ios::app);
#if DEBUG_SINGLE_GRAPH


    std::string s = "Q?hY@eOGG??B_??@g???T?a??@g"; // your test graph6 string
    std::cout << "Running single test graph\n";

    //GraphColoringData data = generateColoring("I?h]@eOWG", 0 ,4);
    //GraphColoringData data = generateColoring("U?GY@COOGCC?@??Ap??a@?G??DK?OA?CE??G?D?G", 1 ,9);
    //GraphColoringData data = generateColoring("[CHI@e?GKOGA???@__OC???_??O_?O?@??H????w@?G??A???A????B_??_O??AH", 6 ,7);
    //GraphColoringData data = generateColoring("Q?hY@eOGG??B_??@g???T?a??@g", 3,5);
    //GraphColoringData data = generateColoring("S?gQ@eOOGC?AP??BO@@?GB????o?E???[", 0,8);
    //GraphColoringData data = generateColoring("c??A@eO?CCC?O??B????hA???@GG??@_C_?GD??G?????O`O??A???__A?@??C?B??G??K???C??????@??CG????????Ag????W?????L", 8,19);

    GraphColoringData data = generateColoring(
        "c?GY?EO?GC??P??BP???_GO???O?E???A?G?OC??a?????F?@?????_C???O?@G???AO?????G??I?????AK??@@?????`G?G_??????E@", 0,
        20);

    // print edges


    //GraphColoringData data = generateColoring(s, 1 ,6);
    exportPython(data.originalSolution, "../export_data/original_coloring.txt", data.originalGraphEdgeList.edge_list,
                 true);

#if CALC_ILP
    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat, data.modifiedGraphEdgeList,
                                                      data.originalSolution, data.baseline, SearchStrategy::ILP);
    exportPython(ilpSolution, "../export_data/ilp_solution.txt", data.originalGraphEdgeList.edge_list, true);

    exportData(data, ilpSolution, SearchStrategy::ILP);


#endif

#if CALC_BRUTEFORCE
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat, data.modifiedGraphEdgeList,
                                                             data.originalSolution, data.baseline,
                                                             SearchStrategy::BruteForce);
    exportPython(bruteforceSolution, "../export_data/bruteforce_solution.txt", data.originalGraphEdgeList.edge_list,
                 true);
#endif


#else

    //string format = "C~";
    std::vector<std::string> filenames = {"../data/4_edge_critical_snarks.18.g6"};

    std::cout << "Working dir: "
            << std::filesystem::current_path() << "\n";

    for (const std::string &filename: filenames) {
        std::ifstream file(filename);

        std::cout << "file is opened: " << filename << std::endl;

        string s;
        int vertex_counter = 1;
        int color_counter = 1;

        while (getline(file, s)) {
            int graph_counter = 1;

            cout << "Working on: #" << graph_counter << endl;
            AdjacencyListGraph graph(s);

            vector<int> vertices = graph.getVertices();
            int all_edges = graph.getEdgeCount();
            vertex_counter = 1;
            for (int i = 0; i < vertices.size(); i++) {
                for (int j = i + 1; j < vertices.size(); j++) {
                    int v1 = vertices.at(i);
                    int v2 = vertices.at(j);
                    if (!graph.containsEdge(Edge(v1, v2))) {
                        continue;
                    }
                    //cout << "Vertex pair: " << v1 << " " << v2 << endl;

#if ALL_COLORS
                    std::vector<GraphColoringData> allData = generateAllColoring(s, v1, v2);
#else
                    std::vector<GraphColoringData> = allData = {generateColoring(s, v1, v2)};
#endif

                    color_counter = 1;
                    for (const GraphColoringData &data: allData) {
                        cout << "Calculating: Vertex[" << vertex_counter << "/" << all_edges << "] Color[" <<
                                color_counter << "/" << allData.size() << "]\n";
                        color_counter++;
                        Solution ilpSolution = findClosestWithDefectThree(
                            data.originalGraphFormat, data.modifiedGraphEdgeList, data.originalSolution, data.baseline,
                            SearchStrategy::ILP);
                        //Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

                        //int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
                        //int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);
                        exportData(data, ilpSolution, SearchStrategy::ILP, outFile);
                    }
                    vertex_counter++;
                }
            }
        }

        file.close();
    }

#endif
    outFile.close();
}
