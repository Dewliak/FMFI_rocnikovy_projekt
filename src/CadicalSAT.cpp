//
// Created by dewliak on 10/13/25.
//

#include "CadicalSAT.h"
#include "../include/CadicalSAT.h"

CadicalSAT::CadicalSAT() {
    solver = CaDiCaL::Solver();
}

void CadicalSAT::add_clause(std::vector<std::pair<int, bool>> literal) {
    for (std::pair<unsigned,bool> l : literal) {
        solver.add(((l.second ? (1) : (-1)))*l.first);
    }
    solver.add(0);
}

SolveResult CadicalSAT::solve() {
    int sat = solver.solve();

    if (sat == 10) {
        return SolveResult::SAT;
    }
    if (sat == 20) {
        return SolveResult::UNSAT;
    }
    return SolveResult::UNKNOWN;
}

bool CadicalSAT::variable_value(int var) {
    return SATInterface::variable_value(var);
}


