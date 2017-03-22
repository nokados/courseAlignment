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
    std::vector<char> alphabet;
    std::map<char, std::size_t> alphabetIndexes;
public:
    ScoreMatrix(std::string text);
    float get(char first, char second);
};


#endif //ONEANOTHERALIGNMENT_SCOREMATRIX_H
