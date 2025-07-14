#pragma once

#include <string>
#include <vector>

namespace utils {

std::string stripMarkdown(const std::string &text);
std::vector<std::string> getSentences(const std::string &text);
void moveIndexPastWhitespace(const std::string &text, size_t &startIndex);
void cleanSentence(std::string &sentence);

} // namespace utils
