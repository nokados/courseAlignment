//
// Created by nokados on 21.03.17.
//

#include "SWAlignment.h"
#include <iostream>

SWAlignment::SWAlignment(ScoreMatrix score, float gap) : score(score), gap(gap) {
}

std::pair<std::wstring, std::wstring>
SWAlignment::align(std::wstring firstSeq, std::wstring secondSeq) {
    this->seq1 = firstSeq;
    this->seq2 = secondSeq;
    this->len1 = firstSeq.size();
    this->len2 = secondSeq.size();

    this->_createSWArray();
    this->_forwardPropagation();
    return this->_backPropagation();
}

void SWAlignment::_createSWArray() {
    this->SWArray.resize(this->len1 + 1, this->len2 + 1, 0);
    this->directions.resize(this->len1 + 1, this->len2 + 1, Directions::NONE);
}

void SWAlignment::_forwardPropagation() {
    this->maxValue = 0;
    this->maxCoords.first = 0;
    this->maxCoords.second = 0;
    for (size_t row = 1; row <= this->len1; row++) {
        for (size_t column = 1; column <= this->len2; column++) {
            this->_updateCellValue(row, column);
        }
    }
}

void SWAlignment::_updateCellValue(size_t row, size_t column) {
    float topValue = this->SWArray.get(row - 1, column) - this->gap;
    float leftValue = this->SWArray.get(row, column - 1) - this->gap;

    wchar_t char1 = this->seq1[row - 1];
    wchar_t char2 = this->seq2[column - 1];
    float topLeftValue = SWArray.get(row - 1, column - 1) + this->score.get(char1, char2);

    float value = 0;
    Directions dir = Directions::NONE;

    if (topValue > value) {
        value = topValue;
        dir = Directions::TOP;
    }
    if (leftValue > value) {
        value = leftValue;
        dir = Directions::LEFT;
    }
    if (topLeftValue > value) {
        value = topLeftValue;
        dir = Directions::TOP_LEFT;
    }

    this->SWArray.set(row, column, value);
    this->directions.set(row, column, dir);

    if (value > this->maxValue) {
        this->maxValue = value;
        this->maxCoords.first = row;
        this->maxCoords.second = column;
    }
}

std::pair<std::wstring, std::wstring> SWAlignment::_backPropagation() {
    std::wstring stringOne = L"", stringTwo = L"";
    Directions dir = this->directions.get(this->maxCoords.first, this->maxCoords.second); //todo вывод если макскоордс не в правом нижнем углу
    std::size_t row = this->maxCoords.first,
            column = this->maxCoords.second;
    while (dir != Directions::NONE) {
        wchar_t char1 = this->seq1[row - 1],
                char2 = this->seq2[column - 1];
        switch (dir) {
            case Directions::LEFT :
                stringOne = L"-" + stringOne;
                stringTwo = char2 + stringTwo;
                column--;
                break;
            case Directions::TOP :
                stringOne = char1 + stringOne;
                stringTwo = L"-" + stringTwo;
                row--;
                break;
            case Directions::TOP_LEFT :
                stringOne = char1 + stringOne;
                stringTwo = char2 + stringTwo;
                column--;
                row--;
                break;
        }
        dir = this->directions.get(row, column);
    }
    return std::make_pair(stringOne, stringTwo);
}

