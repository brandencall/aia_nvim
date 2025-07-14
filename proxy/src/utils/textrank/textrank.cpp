#include "textrank.h"
#include "preprocess_text.h"
#include <cmark.h>
#include <numeric>

namespace utils {

std::string summarizeText(const std::vector<std::string> &text) {
    std::string result;
    std::string full_text =
        std::accumulate(text.begin(), text.end(), std::string(), [](const std::string &acc, const std::string &line) {
            return acc.empty() ? line : acc + "\n" + line;
        });
    std::string plainText = stripMarkdown(full_text);
    std::vector<std::string> sentences = getSentences(plainText);
    return result;
}

} // namespace utils
