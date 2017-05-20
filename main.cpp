#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "SWAlignment.h"
#include "ScoreMatrix.h"
#include "FileManager.h"

/**
 * Entry
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::locale loc("en_US.UTF-8"); // You can also use "" for the default system locale
    std::wcout.imbue(loc); // Use it for output

    FileManager fm("/home/nokados/ClionProjects/courseAlignment/CPPKeywords.ini", loc);
    char* firstFile = argv[1];
    char* secondFile = argv[2];
    std::wstring first = fm.loadCode(firstFile);
    std::wstring second = fm.loadCode(secondFile);

    ScoreMatrix score(first + second);
    for (std::size_t row = 0; row < score.numRows; row++) {
        for (std::size_t column = 0; column < score.numColumns; column++) {
            score.set(row, column, 2 * (row == column));
        }
    }
    score.merge(fm.getScore());

    SWAlignment align = SWAlignment(score);
    auto res = align.align(first, second);
    fm.printAlignedStrings(res.first, res.second);
    return 0;
}