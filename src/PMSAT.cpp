//
// Created by dewliak on 10/28/25.
//

#include "../include/PMSAT.h"

#include <algorithm>
#include <set>
#include <vector>

#include "EdgeList.h"


template<typename T>
std::set<std::set<T>> all_subsets_size_k(std::vector<T> array, int k) {
    // return all subsets of size k
    std::vector<bool> used(array.size(), false);
    for (int i =0; i < k; i++) {
        used[i] = true;
    }

    std::set<std::set<T>> subsets;

    do {
        std::set<T> subset;
        for (int i =0; i < array.size(); i++) {
            if (used[i]) {
                subsets.add(array[i]);
            }
        }
        subsets.add(subset);

    }while (std::next_permutation(used.begin(), used.end()));

    return subsets;
}


void cnf_k(EdgeList edge_list, int k) {

    return;

}


/*
 * THE PLAN:
 *  1. generate perfect matching
 *  2. get defect
 */