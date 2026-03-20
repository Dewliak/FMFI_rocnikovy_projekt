//
// Created by dewliak on 3/11/26.
//

#include "export/exportPython.h"
#include <fstream>
#include <iostream>
void exportPython(Solution solution, std::string filename, vector<Edge> edge_list, bool deleted_edges) {
    // deleted edges are shown the uv pair we've deleted

    std::ofstream out(filename);

    if(!out)
    {
        std::cout << "Error opening file " << filename << "\n";
        return;
    }

    for(const Edge& e : solution.M1)
        out << e.first << " " << e.second << " 0\n";

    for(const Edge& e : solution.M2)
        out << e.first << " " << e.second << " 1\n";

    for(const Edge& e : solution.M3)
        out << e.first << " " << e.second << " 2\n";

    if (!edge_list.empty()){
        for (Edge& e : edge_list) {
            if (!(solution.M1.contains(e) || solution.M2.contains(e) || solution.M3.contains(e))) {
                out << e.first << " " << e.second;
                if (deleted_edges) {
                    out << " 4\n";
                }
                else {
                    out << " 3\n";
                }

            }
        }
    }


    out.close();


}