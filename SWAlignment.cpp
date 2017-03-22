//
// Created by nokados on 21.03.17.
//

#include "SWAlignment.h"
#include <iostream>
#include <algorithm>

SWAlignment::SWAlignment(ScoreMatrix score, float gapInit, float gapContinue) : score(score), gapInit(gapInit),
                                                                                gapContinue(gapContinue) {
}

std::map<char, char>
SWAlignment::align(std::string firstSeq, std::string secondSeq) {
    this->seq1 = firstSeq;
    this->seq2 = secondSeq;
    this->len1 = firstSeq.size();
    this->len2 = secondSeq.size();

    this->_createSWArray();
    this->_forwardPropagation();

    return std::map<char, char>();
}

void SWAlignment::_createSWArray() {
    this->SWArray.resize(this->len1 + 1, this->len2 + 1);
    this->topValues.resize(this->len1 + 1, this->len2 + 1);
    this->leftValues.resize(this->len1 + 1, this->len2 + 1);
}

void SWAlignment::_forwardPropagation() {
    for (size_t row = 1; row <= this->len1; row++) {
        for (size_t column = 1; column <= this->len2; column++) {
            this->_updateCellValue(row, column);
        }
    }
}

void SWAlignment::_updateCellValue(size_t row, size_t column) {
    float topValue = std::max((float) 0, this->topValues.get(row - 1, column) - this->gapContinue);
    topValue = std::max(topValue, this->SWArray.get(row - 1, column) - this->gapInit);
    this->topValues.set(row, column, topValue);

    float leftValue = std::max((float) 0, this->leftValues.get(row, column - 1) - this->gapContinue);
    leftValue = std::max(leftValue, this->SWArray.get(row, column - 1) - this->gapInit);
    this->leftValues.set(row, column, leftValue);

    float value = std::max(leftValue, topValue);
    char char1 = this->seq1[row - 1];
    char char2 = this->seq2[column - 1];
    value = std::max(value, SWArray.get(row - 1, column - 1) + this->score.get(char1, char2));
    this->SWArray.set(row, column, value);
}

