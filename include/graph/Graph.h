//
// Created by dewliak on 10/5/25.
//

#ifndef ROCNIKOVY_PROJEKT_GRAPH_H
#define ROCNIKOVY_PROJEKT_GRAPH_H
#include <string>
#include <vector>

#include "EdgeList.h"
#include "../Matrix.h"

using namespace std;

class Graph {
public:
    explicit Graph(const std::string graph6format);

    Graph(Matrix input_matrix);


    ~Graph();

    void printMatrix();
    vector<vector<int>> getMatrix() const;



    int getVerticesCount() const;
    void copyMatrix(vector<vector<int>> matrix);
    void removeVertice(int u);
    void addEdge(int u, int v, int color = 1);
    void removeEdge(int u, int v);
    bool hasEdge(int u, int v);
    int degree(int u);
    vector<int> neighbors(int u);
    Matrix generateIncidenceMatrix();

private:
    Matrix matrix;
    int vertices;

    void add_transpose();
    void convert_to_matrice(string format);


};


#endif //ROCNIKOVY_PROJEKT_GRAPH_H