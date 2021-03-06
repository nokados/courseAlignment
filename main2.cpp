#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "SWAlignment.h"
#include "ScoreMatrix.h"
#include "FileManager.h"

std::string getTextFromFile(const char* file) ;

int main(int argc, char** argv) {
    FileManager fm("CPPKeywords.ini");
    std::wstring first = fm.loadCode("main.cpp");
    std::wstring second = fm.loadCode("main2.cpp");

    ScoreMatrix score(first + second);
    for (std::size_t row = 0; row < score.numRows; row++) {
        for (std::size_t column = 0; column < score.numColumns; column++) {
            score.set(row, column, row == column);
        }
    }

    SWAlignment align = SWAlignment(score);
    auto res = align.align(first, second);
    std::wcout << res.first << std::endl << res.second << std::endl;

    return 0;
}