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

#include "Pipeline.h"

using namespace std;



///// SO FAR SEEMS GOOD, JUST PROBLEM WITH THE HAMMING DISTANCE, FIRST TRY MIGHT BE BETTER
//// TO BRUTE FORCE ALL SOLUTIONS AND JUST CHECK HAMMING DISTANCE MANUALLYx

#


#define DEBUG_SINGLE_GRAPH 1
#define CALC_BRUTEFORCE 1
#define CALC_ILP 1
int main() {
#if DEBUG_SINGLE_GRAPH

    std::string s = "Q?hY@eOGG??B_??@g???T?a??@g";   // your test graph6 string
    std::cout << "Running single test graph\n";

    //GraphColoringData data = generateColoring("I?h]@eOWG", 0 ,4);
    //GraphColoringData data = generateColoring("U?GY@COOGCC?@??Ap??a@?G??DK?OA?CE??G?D?G", 1 ,9);
    //GraphColoringData data = generateColoring("[CHI@e?GKOGA???@__OC???_??O_?O?@??H????w@?G??A???A????B_??_O??AH", 6 ,7);
    //GraphColoringData data = generateColoring("Q?hY@eOGG??B_??@g???T?a??@g", 3,5);
    //GraphColoringData data = generateColoring("S?gQ@eOOGC?AP??BO@@?GB????o?E???[", 0,8);
    //GraphColoringData data = generateColoring("c??A@eO?CCC?O??B????hA???@GG??@_C_?GD??G?????O`O??A???__A?@??C?B??G??K???C??????@??CG????????Ag????W?????L", 8,19);

    GraphColoringData data = generateColoring("c?GY?EO?GC??P??BP???_GO???O?E???A?G?OC??a?????F?@?????_C???O?@G???AO?????G??I?????AK??@@?????`G?G_??????E@", 0, 20);

    //GraphColoringData data = generateColoring(s, 1 ,6);
    exportPython(data.originalSolution, "../export_data/original_coloring.txt", data.originalGraphEdgeList.edge_list, true);

#if CALC_ILP
    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    exportPython(ilpSolution, "../export_data/ilp_solution.txt", data.originalGraphEdgeList.edge_list, true);
#endif

#if CALC_BRUTEFORCE
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);
    exportPython(bruteforceSolution, "../export_data/bruteforce_solution.txt", data.originalGraphEdgeList.edge_list, true);
#endif




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