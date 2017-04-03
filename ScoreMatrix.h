//
// Created by nokados on 21.03.17.
//

#ifndef ONEANOTHERALIGNMENT_SCOREMATRIX_H
#define ONEANOTHERALIGNMENT_SCOREMATRIX_H

#include <string>
#include <vector>
#include <map>

#include "Matrix.h"

class ScoreMatrix : public Matrix<float> {
private:
    /**
     * Список всех символов текста
     */
    std::vector<wchar_t> alphabet;
    /**
     * Какой индекс имеет символ в списке alphabet
     */
    std::map<wchar_t, std::size_t> alphabetIndexes;
public:
    ScoreMatrix();
    ScoreMatrix(std::wstring text);
    float get(wchar_t first, wchar_t second);
    /**
     * Добавляет новый символ в алфавит.
     * Матрица score для него будет равна matchWeight на диагонале
     * и diffWeight в других ячейках
     * @param newChar
     * @param matchWeight
     * @param diffWeight
     */
    void addChar(wchar_t newChar, float matchWeight = 1, float diffWeight = -1);
    /**
     * Перезаписывает совпадающие ключи новыми значениями
     * @param score
     */
    void merge(ScoreMatrix* anotherScore);

    bool has(wchar_t ch);
};


#endif //ONEANOTHERALIGNMENT_SCOREMATRIX_H
