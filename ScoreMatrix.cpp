//
// Created by nokados on 21.03.17.
//

#include "scoreMatrix.h"
#include <set>

scoreMatrix::scoreMatrix(std::string text) : Matrix() {
    std::set<char> chars;
    chars.insert(text.begin(), text.end());
    this->alphabet = std::vector<char>(chars.begin(), chars.end());
    unsigned long alphabetSize = this->alphabet.size();
    this->resize(alphabetSize, alphabetSize);
    for (size_t index = 0; index < alphabetSize; index++) {
        this->alphabetIndexes.insert(std::make_pair(alphabet[index], index));
    }
}

int scoreMatrix::get(char first, char second) {
    size_t firstIndex = this->alphabetIndexes[first];
    size_t secondIndex = this->alphabetIndexes[second];
    return Matrix::get(firstIndex, secondIndex);
}