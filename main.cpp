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
    FileManager fm("CPPKeywords.ini");
    std::wstring first = fm.loadCode("/home/nokados/ClionProjects/courseAlignment/oC4vwCiSulo9v6Ma/c.arguments.process-args-1/41-000324-Алиев_Магомед-20161217151017.c");
    std::wstring second = fm.loadCode("/home/nokados/ClionProjects/courseAlignment/oC4vwCiSulo9v6Ma/c.arguments.process-args-1/41-000298-Потехин_Сергей-20161210130728.c");

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