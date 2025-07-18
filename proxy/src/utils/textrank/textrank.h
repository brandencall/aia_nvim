#pragma once

#include <string>
#include <vector>

namespace utils {

std::string summarizeText(const std::vector<std::string> &text);
std::vector<std::string> cleanText(const std::vector<std::string> &text);
std::vector<std::vector<double>> buildTextRankGraph(const std::vector<std::vector<double>> &tfidfMatrix);
std::vector<double> textrank(const std::vector<std::vector<double>> &graph);
std::vector<std::pair<std::string, double>> getTopSentences(const std::vector<std::string> &sentences,
                                                            const std::vector<double> &scores, size_t top = 5);
std::string concatinateSentences(const std::vector<std::pair<std::string, double>> &topSentences);

} // namespace utils
