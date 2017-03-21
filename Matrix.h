//
// Created by nokados on 21.03.17.
//

#ifndef ONEANOTHERALIGNMENT_MATRIX_H
#define ONEANOTHERALIGNMENT_MATRIX_H

#include <vector>

template<typename T>
class Matrix {
protected:
    std::vector<T> data;
public:
    std::size_t numRows, numColumns;

    Matrix() : numRows(0), numColumns(0) {}

    Matrix(std::size_t numRows, std::size_t numColumns) : numRows(numRows), numColumns(numColumns) {
        this->data = std::vector<T>(numRows * numColumns, 0);
    }

    T get(std::size_t row, std::size_t column) {
        return this->data[row * numColumns + column];
    }

    void set(std::size_t row, std::size_t column, T value) {
        this->data[row * numColumns + column] = value;
    }

    void resize(std::size_t numRows, std::size_t numColumns) {
        this->data.resize(numRows * numColumns, 0);
        this->numRows = numRows;
        this->numColumns = numColumns;
    }
};


#endif //ONEANOTHERALIGNMENT_MATRIX_H
