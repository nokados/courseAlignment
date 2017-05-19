//
// Created by nokados on 21.03.17.
//

#include "ScoreMatrix.h"
#include <set>
#include <iostream>


ScoreMatrix::ScoreMatrix(std::wstring text) : Matrix() {
    std::set<wchar_t> chars;
    chars.insert(text.begin(), text.end());
    this->alphabet = std::vector<wchar_t>(chars.begin(), chars.end());
    unsigned long alphabetSize = this->alphabet.size();
    this->resize(alphabetSize, alphabetSize, 0);
    for (size_t index = 0; index < alphabetSize; index++) {
        this->alphabetIndexes.insert(std::make_pair(alphabet[index], index));
    }
}

float ScoreMatrix::get(wchar_t first, wchar_t second) {
    size_t firstIndex = this->alphabetIndexes[first];
    size_t secondIndex = this->alphabetIndexes[second];
    return Matrix::get(firstIndex, secondIndex);
}

ScoreMatrix::ScoreMatrix() {}

void ScoreMatrix::addChar(wchar_t newChar, float matchWeight) {
    this->alphabet.push_back(newChar);
    unsigned long alphabetSize = this->alphabet.size();
    this->alphabetIndexes.insert(std::make_pair(alphabet[alphabetSize-1], alphabetSize-1));
    this->set(alphabetSize - 1, alphabetSize - 1, matchWeight);
}

void ScoreMatrix::merge(ScoreMatrix *anotherScore) {
    for (auto it = this->alphabet.begin(); it != this->alphabet.end(); it++) {
        if (anotherScore->has(*it)) {
            std::size_t index = this->alphabetIndexes[*it];
            // Только диагональные элементы
            this->set(index, index, anotherScore->get(*it, *it));
        }
    }
}

bool ScoreMatrix::has(wchar_t ch) {
    return this->alphabetIndexes.find(ch) != this->alphabetIndexes.end();
}
