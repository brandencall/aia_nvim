#include "tfidf.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils {

tfidf::tfidf(const std::vector<std::string> sentences) : sentences{sentences} {
    buildSentenceWordMap(sentences);
    buildWordIndexes();
    matrix = std::vector<std::vector<double>>(sentences.size(), std::vector<double>(wordToIndex.size(), 0.0));
    computeTFIDF();
}

void tfidf::buildSentenceWordMap(const std::vector<std::string> sentences) {
    for (size_t i = 0; i < sentences.size(); i++) {
        sentenceWordMap[i] = getWordsFromSentence(sentences[i]);
    }
}

void tfidf::buildWordIndexes() {
    int index = 0;
    for (const auto &sentence : sentenceWordMap) {
        for (const auto &word : sentence.second) {
            if (wordToIndex.find(word) == wordToIndex.end()) {
                wordToIndex[word] = index;
                indexToWord[index] = word;
                index++;
            }
        }
    }
}

void tfidf::computeTFIDF() {
    calculateTF();
    std::unordered_map<std::string, double> idf = calculateIDF();
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            std::string word = getWordFromIndex(j);
            double idfScore = idf[word];
            matrix[i][j] *= idfScore;
        }
    }
}

int tfidf::getIndexFromWord(const std::string &word) {
    if (wordToIndex.find(word) != wordToIndex.end()) {
        return wordToIndex[word];
    }
    return -1;
}

std::string tfidf::getWordFromIndex(int index) const {
    auto indexWordPair = indexToWord.find(index);
    return (indexWordPair != indexToWord.end()) ? indexWordPair->second : "";
}

std::vector<std::string> tfidf::getWordsFromSentence(const std::string &sentence) {
    std::vector<std::string> words;
    std::string word;
    std::istringstream iss(sentence);
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

void tfidf::calculateTF() {
    for (const auto &sentence : sentenceWordMap) {
        std::unordered_map<std::string, int> wordCounts;
        for (const auto &word : sentence.second) {
            wordCounts[word]++;
        }

        int totalWords = sentence.second.size();
        for (const auto &[word, count] : wordCounts) {
            matrix[sentence.first][getIndexFromWord(word)] = static_cast<double>(count) / totalWords;
        }
    }
}

std::unordered_map<std::string, double> tfidf::calculateIDF() {
    std::unordered_map<std::string, double> idf;
    for (const auto &sentence : sentenceWordMap) {
        for (const auto &word : sentence.second) {
            if (idf.find(word) == idf.end()) {
                idf[word] = calculateIndividualWordIDF(word);
            }
        }
    }
    return idf;
}

double tfidf::calculateIndividualWordIDF(const std::string &word) {
    int wordCount = 0;
    // Using plain sentences because substrings are found with this method.
    // For example, if the word is cat then cats will also count
    for (const auto &sentence : sentences) {
        if (sentence.find(word) != std::string::npos) {
            wordCount++;
        }
    }
    // This shouldn't happen
    if (wordCount == 0) {
        return 0;
    }
    return std::log10(static_cast<double>(sentences.size()) / (wordCount));
}

} // namespace utils
