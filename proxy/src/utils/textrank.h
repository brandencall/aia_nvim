#pragma once

#include <string>
#include <vector>

namespace utils {

std::string summarizeText(const std::vector<std::string> &text);
std::string stripMarkdown(const std::string &text);
std::vector<std::string> getSentences(const std::string &text);
void moveIndexPastWhitespace(const std::string &text, size_t &startIndex);

} // namespace utils
