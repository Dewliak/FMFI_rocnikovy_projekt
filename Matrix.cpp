//
// Created by dewliak on 10/6/25.
//

#include "Matrix.h"
#include <string>
#include <stdexcept>


Matrix::Matrix() : row_count(0), col_count(0) {

}

Matrix::Matrix(vector<vector<int>> matrix) {
    this->row_count = matrix.size();
    this->col_count = matrix[0].size();
    data = matrix;

}

Matrix::Matrix(int row_count, int column_count) {
    this->row_count = row_count;
    this->col_count = column_count;
    data = vector<vector<int>>(row_count, vector<int>(column_count, 0));
}


const int &Matrix::get(const int i, const int j) const {
    if (!inBounds(i, j)) {
        throw runtime_error("Matrix out of bounds");
    }
    return data[i][j];
}

vector<vector<int>> Matrix::get_data() const {
    return data;
}

void Matrix::set(int i, int j, int value) {
    if (!inBounds(i, j)) {
        throw runtime_error("Matrix out of bounds");
    }
    data[i][j] = value;
}

int Matrix::get_row_count() const{
    return row_count;
}

int Matrix::get_col_count() const {
    return col_count;
}

Matrix Matrix::transpose() {
    Matrix result(col_count, row_count);
    for (int i=0; i < col_count; i++) {
        for (int j=0; j < row_count; j++) {
            result.set(i,j,data[j][i]);
        }
    }

    return result;
}

const int&  Matrix::operator()(int i, int j) const {
    return data[i][j];
}

bool Matrix::inBounds(int i, int j) const {
    return (i >= 0 && i < get_row_count()) && (j >= 0 && j < get_col_count());
}

bool Matrix::same_size(const Matrix& other) const {
    return (row_count == other.get_row_count() && col_count == other.get_row_count());
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (!same_size(other)) {
        throw runtime_error("Matrix has to be the same size");
    }
    Matrix result(get_data());

    for (int i =0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            result.set(i,j,result(i,j) + other(i,j));
        }
    }

    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (!same_size(other)) {
        throw runtime_error("Matrix has to be the same size");
    }
    Matrix result(get_data());

    for (int i =0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            result.set(i,j,result(i,j) - other(i,j));
        }
    }

    return result;
}

bool Matrix::operator==(const Matrix& other) const {
    if (!same_size(other)) {
        return false;
    }

    for (int i =0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            if (this->get(i,j) != other(i,j)) {
                return false;
            }
        }
    }
    return true;
}

string Matrix::to_string() const {
    string repr = "";
    for (int i = 0; i < row_count; i++) {
        for (int j =0; j < col_count;j++) {
            repr += std::to_string(get(i,j));
            repr += " ";
        }
        repr += "\n";
    }

    return repr;
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (row_count != other.get_col_count() || col_count != other.get_row_count()) {
        throw runtime_error("Matrixes have to have size NxM and MxN");
    }

    Matrix result(row_count, row_count);

    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < other.get_col_count(); j++) {
            result.set(i,j,0);
            for (int k = 0; k < other.get_row_count(); k++) {
                result.set(i,j,result.get(i,j) + get(i,k) * other(k,j));
                //rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}

Matrix Matrix::operator*(int scalar) const {
    Matrix result(get_data());

    for (int i =0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            result.set(i,j,scalar*result(i,j) );
        }
    }

    return result;
}
