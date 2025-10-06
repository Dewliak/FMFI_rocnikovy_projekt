//
// Created by dewliak on 10/5/25.
// indexing starts from 0
//

#include "Graph.h"

#include <bitset>
#include <vector>
#include <iostream>
#include <stdexcept>

Graph::Graph(const std::string graph6format) {
    convert_to_matrice(graph6format);
}

Graph::~Graph() {
    return;
};

void Graph::printMatrix() {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j];
        }
        cout << endl;
    }

}


void Graph::add_transpose() {
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < i; j++) {
            matrix[j][i] = matrix[i][j];
        }
    }
}

int Graph::getVerticesCount() const {
    return vertices;
}

void Graph::copyMatrix(vector<vector<int>> matrix) {
    throw std::logic_error("Not implemented!");
}

void Graph::removeVertice(int u) {
    // TODO: test
    for (int i = 0; i < vertices;i++) {
        matrix[u][i] = 0;
        matrix[i][u] = 0;
    }

}

void Graph::addEdge(int u, int v, int color) {
    if (u > vertices - 1 || u < 0 || v > vertices - 1 || v < 0) {
        throw "vertex out of range";
    }

    matrix[u][v] = color;
    matrix[v][u] = color;

}

void Graph::removeEdge( int u, int v) {
    if (u > vertices - 1 || u < 0 || v > vertices - 1 || v < 0) {
        throw "vertex out of range";
    }

    matrix[u][v] = 0;
    matrix[v][u] = 0;

}

bool Graph::hasEdge(int u, int v) {
    if (u > vertices - 1 || u < 0 || v > vertices - 1 || v < 0) {
        throw "vertex out of range";
    }

    return (matrix[u][v] != 0);
}

int Graph::degree(int u) {
    throw std::logic_error("Not implemented!");
}

vector<int> Graph::neighbors(int u) {
    throw std::logic_error("Not implemented!");
}

vector<vector<int> > Graph::getMatrix() {
    return matrix;
}



void Graph::convert_to_matrice(string format) {
    vertices = static_cast<int>(format[0]) - 63; // (char)format[0] casts it to a signed char, we need unsigned

    string all_bits = "";
    for (int i = 1; i < format.size(); i++) {
        int value = static_cast<int>(format[i]) - 63;
        bitset<6> bits(value);
        all_bits += bits.to_string();
    }


    matrix = vector(vertices,vector<int>(vertices,0));

    int num_in_bot_left_diag = 0;
    for (int i = 0; i < vertices; i++) {
        num_in_bot_left_diag += i;
    }

    string bot_left_diag = all_bits.substr(0, num_in_bot_left_diag);

    size_t pos = 0;
    for (int i =0; i < vertices; i++) {
        // fill the first i with zeroes, cuz the triangle but the constructor already fills it up

        for (int j = 0; j < i; j++) {
            matrix[i][j] = all_bits[pos++] - '0';
        }

    }

    // add transpose
    add_transpose();

};

