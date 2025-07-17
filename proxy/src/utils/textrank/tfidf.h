#pragma once

#include <array>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
namespace utils {

class tfidf {

  public:
    // 2D vector of doubles. row: sentence, col: words
    std::vector<std::vector<double>> matrix;

    explicit tfidf(const std::vector<std::string> &sentences);

    void buildSentenceWordMap(const std::vector<std::string> &sentences);
    void buildWordIndexes();
    void computeTFIDF();
    int getIndexFromWord(const std::string &word);
    std::string getWordFromIndex(int index) const;
    std::vector<std::string> getWordsFromSentence(const std::string &sentence);
    void calculateTF();
    std::unordered_map<std::string, double> calculateIDF();
    double calculateIndividualWordIDF(const std::string &word);

  private:
    // Using this for the idf calculation
    const std::vector<std::string> &sentences;
    std::unordered_map<std::string, int> wordToIndex;
    std::unordered_map<int, std::string> indexToWord;
    std::unordered_map<int, std::vector<std::string>> sentenceWordMap;
};

} // namespace utils
