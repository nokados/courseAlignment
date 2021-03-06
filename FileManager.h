//
// Created by nokados on 01.04.17.
//

#ifndef ONEANOTHERALIGNMENT_FILEMANAGER_H
#define ONEANOTHERALIGNMENT_FILEMANAGER_H

#include <string>
#include <map>
#include "ScoreMatrix.h"

const wchar_t UNICODE_RESERVE_FIRST = L'\uE000';
const wchar_t UNICODE_RESERVE_LAST = L'\uF8FF';

class FileManager {
private:
    std::map<wchar_t, std::wstring> unicodeToKeywords;
    std::map<wchar_t, std::wstring> hidedText;
    ScoreMatrix score;
    wchar_t lastUnicode;
    std::locale loc;
public:
    /**
     * Инициализирует матрицу перевода ключего слова в символы unicode.
     * В score записываются веса для ключевых слов из файла.
     * @param keywordsFile
     */
    FileManager(const char *keywordsFile, std::locale loc);

    std::wstring loadCode(const char *file);

    ScoreMatrix *getScore();

    void printAlignedStrings(std::wstring firstText, std::wstring secondText);

private:
    std::wstring _getTextFromFile(const char *file);

    /**
     * Заменяет ключевые слова на соответствующие им символы из Unicode
     * Также заменяет все пробельные символы на один пробел
     *
     * @param text
     * @return отформатированную строку
     */
    std::wstring _replaceKeywordsWithUnicode(std::wstring text);

    std::wstring _getOriginalSubstring(wchar_t firstChar, wchar_t secondChar);

    std::wstring _hideText(std::wstring basic_string);

    std::wstring _hideQuote(std::wstring basic_string, wchar_t i, size_t i1);

    std::wstring restoreHiddenText(std::wstring basic_string);

    wchar_t getLastUnicode();
};


#endif //ONEANOTHERALIGNMENT_FILEMANAGER_H
