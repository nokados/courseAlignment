#include <iostream>
#include <string>
#include "SWAlignment.h"
#include "ScoreMatrix.h"

int main(int argc, char** argv) {
    std::string first = "kazaksouloK";
    std::string second = "kazaoulo";
    ScoreMatrix score(first + second);
    for (std::size_t row = 0; row < score.numRows; row++) {
        for (std::size_t column = 0; column < score.numColumns; column++) {
            score.set(row, column, row == column);
        }
    }

    SWAlignment align = SWAlignment(score);
    auto res = align.align(first, second);
    std::cout << res.first << std::endl << res.second << std::endl;
    return 0;
}