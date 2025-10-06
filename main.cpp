#include "cadical.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#include "Graph.h"
#include "ColoringSAT.h"
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
    // D?C - colorable
    // D~? - not colorable
    string format = "I?h]@eOWG";
    Graph g(format);

    g.printMatrix();

    ColoringSAT sat(g, 3);


    std::cout << (sat.solve() ? "SATISFIABLE" : "UNSATISFIABLE") << std::endl;

    vector<int> colors = sat.getColoring();
    for (int i =0; i < colors.size(); i++) {
        cout << i << ": " << colors[i] << endl;

    }

    Matrix m1({{1,1},{2,2},{3,3}});
    Matrix m2({{1,1,1},{2,2,2}});

    Matrix m3 = m1 * m2;
    cout << m3.to_string() << endl;

    return 0;
}
