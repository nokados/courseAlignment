//
// Created by nokados on 21.03.17.
//

#ifndef ONEANOTHERALIGNMENT_SWALIGNMENT_H
#define ONEANOTHERALIGNMENT_SWALIGNMENT_H
#include <vector>
#include <map>
#include <string>

#include "ScoreMatrix.h"


class SWAlignment {
private:
    ScoreMatrix score;
    Matrix<float> SWArray, topValues, leftValues;
    /**
     * Штраф за открытие и продолжение пропуска
     */
    float gapInit, gapContinue;
    std::string seq1, seq2;
    size_t len1, len2;
public:
    SWAlignment(ScoreMatrix score, float gapInit = 1, float gapContinue = 1);
    std::map<char,char> align(std::string firstSeq, std::string secondSeq);

private:
    void _createSWArray();
    void _forwardPropagation();
    void _updateCellValue(size_t row, size_t column);

};


#endif //ONEANOTHERALIGNMENT_SWALIGNMENT_H
