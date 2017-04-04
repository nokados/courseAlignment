//
// Created by nokados on 01.04.17.
//

#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

std::wstring FileManager::_getTextFromFile(const char *file) {
    std::wifstream in(file, std::ios::in);
    if (in) {
        std::wstring text;
        in.seekg(0, std::ios::end);
        text.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&text[0], text.size());
        in.close();
        return (text);
    }
    perror("File error");
    throw (errno);
}

std::wstring FileManager::loadCode(const char *file) {
    std::wstring text = _getTextFromFile(file);
    text = this->_replaceKeywordsWithUnicode(text);
    return text;
}

FileManager::FileManager(const char *keywordsFile) : score() {
    std::wifstream in(keywordsFile, std::ios::in);
    if (!in) {
        perror("File error");
        throw (errno);
    }
    std::wstring pattern;
    float weight;
    wchar_t unicodeChar = UNICODE_RESERVE_FIRST;
    while (in >> pattern >> weight) {
        this->score.addChar(unicodeChar, weight);
        this->unicodeToKeywords.insert(std::make_pair(unicodeChar, pattern));
        unicodeChar++;
        if (unicodeChar > UNICODE_RESERVE_LAST) {
            throw "Not enough reserved Unicode characters";
        }
    }
    in.close();
}


ScoreMatrix *FileManager::getScore() {
    return &this->score;
}

void FileManager::printAlignedStrings(std::wstring firstText, std::wstring secondText) {
    for (std::size_t firstIt = 0; firstIt < firstText.length(); firstIt++) {
        wchar_t secondChar = (secondText.length() > firstIt) ? secondText[firstIt] : L'-';
        std::wcout << this->_getOriginalSubstring(firstText[firstIt], secondChar);
    }
    std::wcout << std::endl;
    for (std::size_t secondIt = 0; secondIt < firstText.length(); secondIt++) {
        wchar_t firstChar = (firstText.length() > secondIt) ? firstText[secondIt] : L'-';
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


std::wstring FileManager::_replaceKeywordsWithUnicode(std::wstring text) {
    std::basic_regex<wchar_t> spaces(L"\\s+");
    text = std::regex_replace(text, spaces, L" ");

    for (auto it = this->unicodeToKeywords.cbegin(); it != this->unicodeToKeywords.cend(); it++) {
        wchar_t unicodeChar = it->first;
        std::wstring rawPattern = it->second;
        // Escape special symbols in pattern
        const std::basic_regex<wchar_t> esc(L"(\\^|\\[|\\]|\\.|\\$|\\{|\\}|\\*|\\(|\\)|\\\\|\\+|\\||\\?|\\<|\\>)");
        std::wstring escapedPattern = std::regex_replace(rawPattern, esc, L"\\$1");
        // Replace pattern with Unicode characters
        std::basic_regex<wchar_t> pattern(escapedPattern);
        std::wstring replace;
        replace.push_back(unicodeChar);
        text = std::regex_replace(text, pattern, replace);
    }

    return text;
}

