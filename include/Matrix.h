//
// Created by dewliak on 10/6/25.
//

#ifndef ROCNIKOVY_PROJEKT_MATRIX_H
#define ROCNIKOVY_PROJEKT_MATRIX_H

#include <vector>
#include <string>
using namespace std;

/*
 * NxM matrix, consisting of integers
 *
 *
 */

class Matrix {
private:
    int row_count;
    int col_count;
    vector<vector<int>> data;
public:
    // (i,j) == (row,col)
    Matrix();
    explicit Matrix(vector<vector<int>> matrix);
    Matrix(int row_count, int column_count);

    const int &get(const int i, const int j) const;
    void set(int i, int j, int value);
    int get_row_count() const;
    int get_col_count() const;
    vector<vector<int>> get_data() const;
    Matrix transpose();
    const int& operator()(int i, int j) const;
    bool inBounds(int i, int j) const;

    bool same_size( const Matrix &other) const;

    string to_string() const;
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    bool operator==(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(int scalar) const;

    static Matrix generateIncidenceMatrix(const Matrix& matrix);
    static bool isSymetrical(const Matrix& matrix);
    static bool isSquare(const Matrix& matrix);
    static Matrix identity(size_t);
};


#endif //ROCNIKOVY_PROJEKT_MATRIX_H