#include "cadical.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#include "../include/graph/Graph.h"
#include "../include/sat/ColoringSAT.h"
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
    string format = "C~";
    Graph g(format);

    vector<vector<int>> vm = g.getMatrix();
    Matrix m(vm);

    Matrix b = Matrix::generateIncidenceMatrix(m);
    Matrix b_t = b.transpose();
    Matrix I = Matrix::identity(b.get_col_count());
    Matrix ans = b_t * b - I * 2;

    cout << ans.to_string() << endl;
    Graph g2(ans);

    ColoringSAT sat(g2, 3);


    std::cout << (sat.solve() ? "SATISFIABLE" : "UNSATISFIABLE") << std::endl;

    /*
    g.printMatrix();

    ColoringSAT sat(g, 3);


    std::cout << (sat.solve() ? "SATISFIABLE" : "UNSATISFIABLE") << std::endl;

    vector<int> colors = sat.getColoring();
    for (int i =0; i < colors.size(); i++) {
        cout << i << ": " << colors[i] << endl;

    }

    Matrix m1({
        {0,1,0,0,0},
        {1,0,1,1,0},
        {0,1,0,1,0},
        {0,1,1,0,1},
        {0,0,0,1,0}});
    Matrix m3 = Matrix::generateIncidenceMatrix(m1);
    cout << m3.to_string() << endl;
    Matrix m3_t = m3.transpose();

    Matrix identity = Matrix::identity(m1.get_row_count());

    Matrix ans = m3_t * m3 - identity*2;

    cout << ans.to_string() << endl;

    Graph g2(ans);
    ColoringSAT sat2(g2, 3);
    std::cout << (sat.solve() ? "SATISFIABLE" : "UNSATISFIABLE") << std::endl;
    */
    return 0;
}
