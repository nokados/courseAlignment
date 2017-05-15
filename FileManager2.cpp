#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

std::wstring FileManager::_getStringsFromFile(const char *file) {
    std::wifstream inputFile(file, std::ios::in);
    if (inputFile) {
        std::wstring text;
        in.seekg(0, std::ios::end);
        text.resize(inputFile.tellg());
        inputFile.seekg(0, std::ios::beg);
        inputFile.read(&text[0], text.size());
        inputFile.close();
        return (text);
    }
    perror("File error");
    throw (errno);
}

std::wstring FileManager::loadCode(const char *filename) {
    std::wstring strings = _getStringsFromFile(filename);
    strings = this->_rkwu(strings);
    return strings;
}

FileManager::FileManager(const char *keywordsFilename) : score() {
    std::wifstream in(keywordsFilename, std::ios::in);
    if (!in) {
        perror("File error");
        throw (errno);
    }
    std::wstring p;
    float w;
    wchar_t uc = UNICODE_RESERVE_FIRST;
    while (in >> p >> w) {
        this->score.addChar(uc, w);
        this->unicodeToKeywords.insert(std::make_pair(uc, p));
        uc++;
        if (uc > UNICODE_RESERVE_LAST) {
            throw "Not enough reserved Unicode characters";
        }
    }
    in.close();
}


ScoreMatrix *FileManager::getScore() {
    return &this->score;
}

void FileManager::printAlignedStrings(std::wstring text1, std::wstring secondText) {
    for (std::size_t firstIt = 0; firstIt < text1.length(); firstIt++) {
        wchar_t secondChar = (secondText.length() > firstIt) ? secondText[firstIt] : L'-';
        std::wcout << this->_getOriginalSubstring(text1[firstIt], secondChar);
    }
    std::wcout << std::endl;
    for (std::size_t secondIt = 0; secondIt < text1.length(); secondIt++) {
        wchar_t firstChar = (text1.length() > secondIt) ? text1[secondIt] : L'-';
        std::wcout << this->_getOriginalSubstring(secondText[secondIt], firstChar);
    }
    std::wcout << std::endl;
}

std::wstring FileManager::_getOriginalSubstring(wchar_t firstChar, wchar_t secondChar) {
    bool isFirstUnicode = this->unicodeToKeywords.find(firstChar) != this->unicodeToKeywords.end();
    bool isSecondUnicode = this->unicodeToKeywords.find(secondChar) != this->unicodeToKeywords.end();
    if(!isFirstUnicode && !isSecondUnicode) {
        std::wstring res;
        res.push_back(firstChar);
        return res;
    }
    if (isFirstUnicode && !isSecondUnicode) {
        return this->unicodeToKeywords[firstChar];
    }
    if (!isFirstUnicode && isSecondUnicode) {
        std::wstring res,
            secondStr = this->unicodeToKeywords[secondChar];
        res.push_back(firstChar);
        for (std::size_t i = 1; i < secondStr.length(); i++) {
            res.push_back(L'-');
        }
        return res;
    }
    if (isFirstUnicode && isSecondUnicode) {
        std::wstring firstStr = this->unicodeToKeywords[firstChar],
            secondStr = this->unicodeToKeywords[secondChar];
        for (std::size_t i = firstStr.length(); i < secondStr.length(); i++) {
            firstStr.push_back(L'-');
        }
        return firstStr;
    }
}


std::wstring FileManager::_rkwu(std::wstring text) {
    std::basic_regex<wchar_t> spaces(L"\\s+");
    text = std::regex_replace(text, spaces, L" ");

    for (auto it = this->unicodeToKeywords.cbegin(); it != this->unicodeToKeywords.cend(); it++) {
        wchar_t uc = it->first;
        std::wstring rawPattern = it->second;
        // Escape special symbols in pattern
        const std::basic_regex<wchar_t> esc(L"(\\^|\\[|\\]|\\.|\\$|\\{|\\}|\\*|\\(|\\)|\\\\|\\+|\\||\\?|\\<|\\>)");
        std::wstring escapedPattern = std::regex_replace(rawPattern, esc, L"\\$1");
        // Replace pattern with Unicode characters
        std::basic_regex<wchar_t> pattern(escapedPattern);
        std::wstring replace;
        replace.push_back(uc);
        text = std::regex_replace(text, pattern, replace);
    }

    return text;
}

