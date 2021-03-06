//
// Created by nokados on 21.03.17.
//

#include "SWAlignment.h"
#include <iostream>

SWAlignment::SWAlignment(ScoreMatrix score, float gapOpen, float gapExtend) : score(score), gapOpen(gapOpen),
                                                                              gapExtend(gapExtend) {
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
    this->topGaps.resize(this->len1 + 1, this->len2 + 1, 0);
    this->leftGaps.resize(this->len1 + 1, this->len2 + 1, 0);
    this->directions.resize(this->len1 + 1, this->len2 + 1, Directions::NONE);
}

void SWAlignment::_forwardPropagation() {
    for (size_t row = 1; row <= this->len1; row++) {
        for (size_t column = 1; column <= this->len2; column++) {
            this->_updateCellValue(row, column);
        }
    }
    this->maxValue = this->SWArray.get(this->len1, this->len2);
    this->maxCoords.first = this->len1;
    this->maxCoords.second = this->len2;

    for (size_t column = 1; column < this->len2; column++) {
        this->_updateMaxValue(this->len1, column);
    }
    for (size_t row = 1; row < this->len1; row++) {
        this->_updateMaxValue(row, this->len2);
    }
}

void SWAlignment::_updateCellValue(size_t row, size_t column) {
    wchar_t char1 = this->seq1[row - 1];
    wchar_t char2 = this->seq2[column - 1];

    float topValue = this->SWArray.get(row - 1, column) - this->_getGap(row, column, Directions::TOP);
    float leftValue = this->SWArray.get(row, column - 1) - this->_getGap(row, column, Directions::LEFT);
    float topLeftValue = SWArray.get(row - 1, column - 1) + this->score.get(char1, char2);

    float value = 0;
    Directions dir = Directions::NONE;

    if (topLeftValue > value) {
        value = topLeftValue;
        dir = Directions::TOP_LEFT;
    }

    if (topValue > value) {
        value = topValue;
        dir = Directions::TOP;
    }
    if (leftValue > value) {
        value = leftValue;
        dir = Directions::LEFT;
    }


    if (dir == Directions::TOP) {
        this->topGaps.set(row, column, this->topGaps.get(row - 1, column) + 1);
    }
    if (dir == Directions::LEFT) {
        this->topGaps.set(row, column, this->leftGaps.get(row, column - 1) + 1);
    }
    this->SWArray.set(row, column, value);
    this->directions.set(row, column, dir);
}

void SWAlignment::_updateMaxValue(size_t row, size_t column) {
    float value = this->SWArray.get(row, column);
    if (value > maxValue) {
        this->maxValue = value;
        this->maxCoords.first = row;
        this->maxCoords.second = column;
    }
}

std::pair<std::wstring, std::wstring> SWAlignment::_backPropagation() {
    std::wstring stringOne = L"", stringTwo = L"";
    Directions dir = this->directions.get(this->maxCoords.first, this->maxCoords.second);
    std::size_t row = this->maxCoords.first,
            column = this->maxCoords.second;

    if (row < this->len1) {
        for (std::size_t i = this->len1; i > row; i--) {
            wchar_t char1 = this->seq1[i - 1];
            stringOne = char1 + stringOne;
            stringTwo = L"-" + stringTwo;
        }
    }

    if (column < this->len2) {
        for (std::size_t i = this->len2; i > column; i--) {
            wchar_t char2 = this->seq2[i - 1];
            stringTwo = char2 + stringTwo;
            stringOne = L"-" + stringOne;
        }
    }

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

float SWAlignment::_getGap(size_t row, size_t column, Directions dir) {
    size_t numGaps;
    if (dir == Directions::TOP) {
        numGaps = this->topGaps.get(row - 1, column);
    } else {
        numGaps = this->leftGaps.get(row, column - 1);
    }

    return this->gapOpen + numGaps * gapExtend;
}

