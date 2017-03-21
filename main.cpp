#include <iostream>
#include "SWAlignment.h"
#include "ScoreMatrix.h"

int main(int argc, char** argv) {
    SWAlignment align = SWAlignment();
    ScoreMatrix score("lolo");

    for (std::size_t row = 0; row < score.numRows; row++) {
        for (std::size_t column = 0; row < score.numColumns; row++) {
            score.set(row, column, row == column);
        }
    }

    return 0;
}