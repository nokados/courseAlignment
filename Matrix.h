//
// Created by nokados on 21.03.17.
//

#ifndef ONEANOTHERALIGNMENT_MATRIX_H
#define ONEANOTHERALIGNMENT_MATRIX_H

#include <vector>
#include <string>
#include <sstream>

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

    void resize(std::size_t numRows, std::size_t numColumns, T defaultValue) {
        this->data.resize(numRows * numColumns, defaultValue);
        this->numRows = numRows;
        this->numColumns = numColumns;
    }

    std::string toString() {
        std::ostringstream res;
        for (size_t i = 0; i < this->data.size(); i++) {
            res << this->data[i] << (((i + 1) % this->numColumns > 0) ? "\t" : "\n");
        }
        return res.str();
    }
};


#endif //ONEANOTHERALIGNMENT_MATRIX_H
