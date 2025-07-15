#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
namespace utils {

class tfidf {

  public:
    // Maps sentence index: <wordIndex, tfidf score>
    std::unordered_map<int, std::unordered_map<int, double>> tfidfMatrix;
    const std::vector<std::string> sentences;

    tfidf(const std::vector<std::string> sentences) : sentences{sentences} {}

    std::unordered_map<int, std::unordered_map<int, double>> computeTFIDF();
    int getOrAddIndex(const std::string &word);
    std::string getWordFromIndex(int index) const;
    std::vector<std::string> getWordsFromSentence(const std::string &sentence);
    std::unordered_map<int, std::unordered_map<std::string, double>> calculateTF();
    std::unordered_map<std::string, double> calculateIDF();
    double calculateIndividualWordIDF(const std::string &word);

  private:
    int nextIndex = 0;
    std::unordered_map<std::string, int> wordToIndex;
    std::unordered_map<int, std::string> indexToWord;
};

} // namespace utils
