#include "tfidf.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils {

std::unordered_map<int, std::unordered_map<std::string, double>> tfidf::computeTFIDF() {
    std::unordered_map<int, std::unordered_map<std::string, double>> result;
    std::unordered_map<int, std::unordered_map<std::string, double>> tf = calculateTF();
    std::unordered_map<std::string, double> idf = calculateIDF();
    for (const auto &sentence : tf) {
        std::unordered_map<std::string, double> tfIdfSentenceScores;
        for (const auto &tfScoreMap : sentence.second) {
            auto idfPair = idf.find(tfScoreMap.first);
            if (idfPair == nullptr) {
                continue;
            }
            // int wordIndex = getOrAddIndex(tfScoreMap.first);
            tfIdfSentenceScores[tfScoreMap.first] = idfPair->second * tfScoreMap.second;
        }
        result[sentence.first] = tfIdfSentenceScores;
    }
    return result;
}

int tfidf::getOrAddIndex(const std::string &word) {
    if (wordToIndex.find(word) == wordToIndex.end()) {
        int index = nextIndex;
        wordToIndex[word] = index;
        indexToWord[index] = word;
        nextIndex++;
    }
    return wordToIndex[word];
}

std::string tfidf::getWordFromIndex(int index) const {
    auto result = indexToWord.find(index);
    if (result != indexToWord.end())
        return result->second;
    return "";
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
    for (size_t i = 0; i < sentences.size(); ++i) {
        const std::string &sentence = sentences[i];
        // TODO: Should just make a list of words at the start of the algorithm
        std::vector<std::string> words = getWordsFromSentence(sentence);

        std::unordered_map<std::string, int> wordCounts;
        for (const auto &word : words) {
            wordCounts[word]++;
        }

        int totalWords = words.size();
        for (const auto &[word, count] : wordCounts) {
            tf[i][word] = static_cast<double>(count) / totalWords;
        }
    }

    return tf;
}

std::unordered_map<std::string, double> tfidf::calculateIDF() {
    std::unordered_map<std::string, double> idf;
    for (const auto &sentence : sentences) {
        // TODO: Should just make a list of words at the start of the algorithm
        std::vector<std::string> words = getWordsFromSentence(sentence);
        for (const auto &word : words) {
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
