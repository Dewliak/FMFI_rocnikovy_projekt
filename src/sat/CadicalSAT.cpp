//
// Created by dewliak on 10/13/25.
//


#include "sat/CadicalSAT.h"

#include <algorithm>
#include <cadical.hpp>
#include <format>
#include <iostream>
#include <memory>

CadicalSAT::CadicalSAT() {
    solver = std::make_unique<CaDiCaL::Solver>();
}

void CadicalSAT::add_clause(std::vector<std::pair<int, bool>> literal) {

    for (std::pair<int,bool> l : literal){
        max_lit = std::max(max_lit,l.first);
        solver->add(((l.second ? (1) : (-1)) * l.first));
    }
    solver->add(0);
}

SolveResult CadicalSAT::solve() {
    int sat = solver->solve();

    if (sat == 10) {
        return SolveResult::SAT;
    }
    if (sat == 20) {
        return SolveResult::UNSAT;
    }
    return SolveResult::UNKNOWN;
}

bool CadicalSAT::variable_value(int var) {
    return solver->val(var) > 0;
}

std::vector<bool> CadicalSAT::getAnswer() {

    std::vector<bool> answer;

    for (int i = 0; i <= max_lit; i++) {
        answer.push_back(variable_value(i));
    }

    return answer;

}


