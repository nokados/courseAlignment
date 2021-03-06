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

/**
 * Smith-Waterman alignment
 */
class SWAlignment {
private:
    ScoreMatrix score;
    Matrix<float> SWArray;
    Matrix<size_t> topGaps, leftGaps;
    Matrix<Directions> directions;
    /**
     * Штраф за открытие и продолжение пропуска
     */
    float gapOpen, gapExtend;
    std::wstring seq1, seq2;
    size_t len1, len2;

    float maxValue;
    std::pair<size_t, size_t> maxCoords;
public:
    SWAlignment(ScoreMatrix score, float gapOpen = 2.5, float gapExtend = 0.2);
    std::pair<std::wstring, std::wstring> align(std::wstring firstSeq, std::wstring secondSeq);

private:
    float _getGap(size_t row, size_t column, Directions dir);
    void _createSWArray();
    void _forwardPropagation();
    void _updateCellValue(size_t row, size_t column);
    std::pair<std::wstring, std::wstring> _backPropagation();

    /**
     * Проверяет, если значение в клетке (row, column) больше сохраненного максимального значение,
     * то последнее перезаписывается
     * @param row
     * @param column
     */
    void _updateMaxValue(size_t row, size_t column);
};


#endif //ONEANOTHERALIGNMENT_SWALIGNMENT_H
