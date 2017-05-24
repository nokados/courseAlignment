//
// Created by nokados on 01.04.17.
//

#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::wstring FileManager::_getTextFromFile(const char *file) {
    std::wifstream in(file, std::ios::in);
    in.imbue(this->loc);
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

FileManager::FileManager(const char *keywordsFile, std::locale loc) : score(), loc(loc) {
    std::wifstream in(keywordsFile, std::ios::in);
    in.imbue(loc);
    if (!in) {
        perror("File error");
        throw (errno);
    }
    std::wstring pattern;
    float weight;
    this->lastUnicode = UNICODE_RESERVE_FIRST;
    std::vector<std::wstring> patterns;
    std::vector<float> weights;
    while (in >> pattern >> weight) {
        patterns.push_back(pattern);
        weights.push_back(weight);
    }
    this->score.resize(patterns.size(), patterns.size(), 0);
    for (size_t i = 0; i < patterns.size(); i++) {
        pattern = patterns[i];
        weight = weights[i];
        wchar_t unicodeChar = this->getLastUnicode();
        this->score.addChar(unicodeChar, weight);
        this->unicodeToKeywords.insert(std::make_pair(unicodeChar, pattern));
    }

    in.close();
}


ScoreMatrix *FileManager::getScore() {
    return &this->score;
}

void FileManager::printAlignedStrings(std::wstring firstText, std::wstring secondText) {
    size_t numLetters = 1, numCorrect = 1;
    std::wostringstream first, second;
    std::wstring str1, str2;
    for (std::size_t firstIt = 0; firstIt < firstText.length(); firstIt++) {
        wchar_t secondChar = (secondText.length() > firstIt) ? secondText[firstIt] : L'-';
        first << this->_getOriginalSubstring(firstText[firstIt], secondChar);
    }
    for (std::size_t secondIt = 0; secondIt < secondText.length(); secondIt++) {
        wchar_t firstChar = (firstText.length() > secondIt) ? firstText[secondIt] : L'-';
        second << this->_getOriginalSubstring(secondText[secondIt], firstChar);
    }
    str1 = first.str();
    str2 = second.str();
    for (int i = 0; i < std::min(str1.length(), str2.length()); i++) {
        numLetters++;
        numCorrect += (str1[i] == str2[i]);
    }
    std::wcout << str1 << std::endl << str2 << std::endl;
    std::wcout << L"Similarity: " << ((float) numCorrect / (float) numLetters) << std::endl;
}

std::wstring FileManager::_getOriginalSubstring(wchar_t firstChar, wchar_t secondChar) {
    bool isFirstUnicode = this->unicodeToKeywords.find(firstChar) != this->unicodeToKeywords.end();
    bool isSecondUnicode = this->unicodeToKeywords.find(secondChar) != this->unicodeToKeywords.end();
    if (!isFirstUnicode && !isSecondUnicode) {
        std::wstring res;
        res.push_back(firstChar);
        return res;
    }
    if (isFirstUnicode && !isSecondUnicode) {
        return this->unicodeToKeywords[firstChar];
    }
    if (!isFirstUnicode) { // && isSecondUnicode
        std::wstring res,
                secondStr = this->unicodeToKeywords[secondChar];
        res.push_back(firstChar);
        for (std::size_t i = 1; i < secondStr.length(); i++) {
            res.push_back(L'-');
        }
        return res;
    }
//    if (isFirstUnicode && isSecondUnicode) {
    std::wstring firstStr = this->unicodeToKeywords[firstChar],
            secondStr = this->unicodeToKeywords[secondChar];
    for (std::size_t i = firstStr.length(); i < secondStr.length(); i++) {
        firstStr.push_back(L'-');
    }
    return firstStr;
}


std::wstring FileManager::_replaceKeywordsWithUnicode(std::wstring text) {
    text = this->_hideText(text);
    std::basic_regex<wchar_t> spaces(L"\\s+");
    text = std::regex_replace(text, spaces, L" ");
    for (auto it = this->unicodeToKeywords.cbegin(); it != this->unicodeToKeywords.cend(); it++) {
        wchar_t unicodeChar = it->first;
        std::wstring rawPattern = it->second;
        // Escape special symbols in pattern
        const std::basic_regex<wchar_t> esc(L"(\\^|\\[|\\]|\\.|\\$|\\{|\\}|\\*|\\(|\\)|\\\\|\\+|\\||\\?|\\<|\\>)");
        std::wstring escapedPattern = std::regex_replace(rawPattern, esc, L"\\$1");
        const std::basic_regex<wchar_t> word(L"\\w+");
        if (escapedPattern[0] == L'#') {
            escapedPattern += L"\\b";
        } else if (std::regex_match(escapedPattern, word)) {
            escapedPattern = L"\\b" + escapedPattern + L"\\b";
        }
        // Replace pattern with Unicode characters
        std::basic_regex<wchar_t> pattern(escapedPattern);
        std::wstring replace;
        replace.push_back(unicodeChar);
        text = std::regex_replace(text, pattern, replace);
    }
    text = restoreHiddenText(text);

    return text;
}

std::wstring FileManager::_hideText(std::wstring fullText) {
    for (std::size_t pos = 0; pos < fullText.length(); pos++) {
        if (fullText[pos] == L'\'') {
            fullText = this->_hideQuote(fullText, L'\'', pos);
            pos += 2;
        } else if (fullText[pos] == L'"') {
            fullText = this->_hideQuote(fullText, L'"', pos);
            pos += 2;
        } else if ((fullText[pos] == L'/') && ((pos + 1) < fullText.length())) {
            if (fullText[pos + 1] == L'/') {
                size_t secondPos = fullText.find(L'\n', pos + 2);
                fullText.erase(pos, secondPos - pos);
            } else if (fullText[pos + 1] == L'*') {
                size_t secondPos = fullText.find(L"*/", pos + 2);
                fullText.erase(pos, secondPos - pos + 2);
            }
        }
    }
    return fullText;
}


std::wstring FileManager::_hideQuote(std::wstring text, wchar_t symbol, size_t pos) {
    bool secondIsEscaped = true;
    size_t secondPos = pos;
    while (secondIsEscaped) {
        secondPos = text.find(symbol, secondPos + 1);
        if (secondPos == std::string::npos) {
            throw ("Не закрытая кавычка");
        }
        // Check escaping
        size_t numSlashes = 0,
                slashPos = secondPos - 1;
        while (text[slashPos] == L'\\') {
            numSlashes++;
            slashPos--; // Дойдет до pos, где заведомо стоит кавычка
        }
        secondIsEscaped = (numSlashes % 2) == 1;
    }
    wchar_t unicodeLabel = this->getLastUnicode();
    std::wstring erased = text.substr(pos + 1, secondPos - pos - 1);
    text.erase(pos + 1, secondPos - pos - 1);
    text.insert(pos + 1, 1, unicodeLabel);
    this->hidedText.insert(std::make_pair(unicodeLabel, erased));
    return text;
}

std::wstring FileManager::restoreHiddenText(std::wstring text) {
    for (auto it = this->hidedText.cbegin(); it != this->hidedText.cend(); it++) {
        wchar_t unicodeChar = it->first;
        std::wstring restoredString = it->second;
        std::wstring repPattern;
        repPattern.push_back(unicodeChar);
        std::basic_regex<wchar_t> pattern(repPattern);
        text = std::regex_replace(text, pattern, restoredString);
    }
    hidedText.clear();
    return text;
}

wchar_t FileManager::getLastUnicode() {
    if (lastUnicode > UNICODE_RESERVE_LAST) {
        throw "Not enough reserved Unicode characters";
    }
    wchar_t unicodeChar = lastUnicode;
    lastUnicode++;
    return unicodeChar;
}

