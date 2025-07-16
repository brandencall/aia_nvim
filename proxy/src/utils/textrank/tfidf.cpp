#include "tfidf.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils {

tfidf::tfidf(const std::vector<std::string> sentences) : sentences{sentences} {
    for (size_t i = 0; i < sentences.size(); i++) {
        sentenceWordMap[i] = getWordsFromSentence(sentences[i]);
    }
}

//May not  need to keep a persistant tfidfMatrix within the class
std::unordered_map<int, std::unordered_map<std::string, double>> tfidf::computeTFIDF() {
    tfidfMatrix = calculateTF();
    std::unordered_map<std::string, double> idf = calculateIDF();
    for (const auto &sentenceEntry : tfidfMatrix) {
        std::unordered_map<std::string, double> tfIdfSentenceScores;
        for (const auto &tfScoreMap : sentenceEntry.second) {
            auto idfPair = idf.find(tfScoreMap.first);
            if (idfPair == nullptr) {
                continue;
            }
            tfIdfSentenceScores[tfScoreMap.first] = idfPair->second * tfScoreMap.second;
        }
        tfidfMatrix[sentenceEntry.first] = tfIdfSentenceScores;
    }
    return tfidfMatrix;
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

std::unordered_map<int, std::unordered_map<std::string, double>> tfidf::calculateTF() {
    std::unordered_map<int, std::unordered_map<std::string, double>> tf;
    for (const auto &sentence : sentenceWordMap) {
        std::unordered_map<std::string, int> wordCounts;
        for (const auto &word : sentence.second) {
            wordCounts[word]++;
        }

        int totalWords = sentence.second.size();
        for (const auto &[word, count] : wordCounts) {
            tf[sentence.first][word] = static_cast<double>(count) / totalWords;
        }
    }
    return tf;
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
