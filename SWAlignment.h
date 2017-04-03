//
// Created by nokados on 21.03.17.
//

#ifndef ONEANOTHERALIGNMENT_SWALIGNMENT_H
#define ONEANOTHERALIGNMENT_SWALIGNMENT_H
#include <vector>
#include <string>

#include "ScoreMatrix.h"

enum class Directions {
    NONE,
    TOP,
    LEFT,
    TOP_LEFT
};

class SWAlignment {
private:
    ScoreMatrix score;
    Matrix<float> SWArray;
    Matrix<Directions> directions;
    /**
     * Штраф за открытие и продолжение пропуска
     */
    float gap;
    std::wstring seq1, seq2;
    size_t len1, len2;

    float maxValue;
    std::pair<size_t, size_t> maxCoords;
public:
    SWAlignment(ScoreMatrix score, float gap = 1.5);
    std::pair<std::wstring, std::wstring> align(std::wstring firstSeq, std::wstring secondSeq);

private:
    void _createSWArray();
    void _forwardPropagation();
    void _updateCellValue(size_t row, size_t column);
    std::pair<std::wstring, std::wstring> _backPropagation();

};


#endif //ONEANOTHERALIGNMENT_SWALIGNMENT_H
