//
// Created by dewliak on 10/13/25.
//

#ifndef ROCNIKOVY_PROJEKT_CADICALSAT_H
#define ROCNIKOVY_PROJEKT_CADICALSAT_H


#include "SATInterface.h"
#include "cadical.hpp"

#include <vector>
#include <utility>

class CadicalSAT: public SATInterface {
public:
    CadicalSAT();
    CadicalSAT(const CadicalSAT&) = delete;
    CadicalSAT& operator=(const CadicalSAT&) = delete;

    void add_clause(std::vector<std::pair<int,bool>> literal ) override;
    //virtual add(unsigned var, boolean value);
    SolveResult solve() override;
    bool variable_value(int var) override;
private:
    CaDiCaL::Solver solver;
};



#endif //ROCNIKOVY_PROJEKT_CADICALSAT_H

