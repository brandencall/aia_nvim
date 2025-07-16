#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
namespace utils {

class tfidf {

  public:
    // Maps sentence index: <word, tfidf score>
    std::unordered_map<int, std::unordered_map<std::string, double>> tfidfMatrix;
    const std::vector<std::string> sentences;

    explicit tfidf(const std::vector<std::string> sentences);

    std::unordered_map<int, std::unordered_map<std::string, double>> computeTFIDF();
    std::vector<std::string> getWordsFromSentence(const std::string &sentence);
    std::unordered_map<int, std::unordered_map<std::string, double>> calculateTF();
    std::unordered_map<std::string, double> calculateIDF();
    double calculateIndividualWordIDF(const std::string &word);

  private:
    int nextIndex = 0;
    std::unordered_map<int, std::vector<std::string>> sentenceWordMap;
};

} // namespace utils
