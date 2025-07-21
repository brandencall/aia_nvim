#include "preprocess_text.h"
#include <algorithm>
#include <cctype>
#include <cmark.h>

namespace utils {

std::string stripMarkdown(const std::string &text) {
    cmark_parser *parser = cmark_parser_new(CMARK_OPT_DEFAULT);
    cmark_parser_feed(parser, text.c_str(), text.size());
    cmark_node *document = cmark_parser_finish(parser);
    // Traverse AST and collect plain text
    std::string result;
    cmark_iter *iter = cmark_iter_new(document);
    cmark_event_type ev_type;
    while ((ev_type = cmark_iter_next(iter)) != CMARK_EVENT_DONE) {
        cmark_node *node = cmark_iter_get_node(iter);
        if (ev_type == CMARK_EVENT_ENTER) {
            if (cmark_node_get_type(node) == CMARK_NODE_TEXT) {
                const char *literal = cmark_node_get_literal(node);
                if (literal) {
                    result += literal;
                }
            }
            // Add newline for block-level elements (e.g., paragraphs, headers)
            else if (cmark_node_get_type(node) == CMARK_NODE_PARAGRAPH ||
                     cmark_node_get_type(node) == CMARK_NODE_HEADING || cmark_node_get_type(node) == CMARK_NODE_ITEM) {
                if (!result.empty() && result.back() != '\n') {
                    result += "\n";
                }
            }
        }
    }
    cmark_iter_free(iter);
    cmark_node_free(document);
    cmark_parser_free(parser);
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    return result;
}

std::vector<std::string> getSentences(const std::string &text) {
    std::vector<std::string> result;
    size_t startIndex = 0;

    while (startIndex < text.length()) {
        size_t endOfSentence = text.find_first_of(".!?\n", startIndex);
        if (endOfSentence == std::string::npos) {
            std::string lastSentence = text.substr(startIndex);
            if (!lastSentence.empty()) {
                cleanSentence(lastSentence);
                result.push_back(lastSentence);
            }
            break;
        } else if (endOfSentence == startIndex) {
            startIndex++;
            continue;
        }
        moveIndexPastWhitespace(text, startIndex);
        size_t length = endOfSentence - startIndex;
        std::string sentence = text.substr(startIndex, length);
        if (sentence.length() <= 1) {
            startIndex = endOfSentence + 1;
            continue;
        }
        cleanSentence(sentence);
        result.push_back(sentence);
        startIndex = endOfSentence + 1;
    }
    return result;
}

void moveIndexPastWhitespace(const std::string &text, size_t &startIndex) {
    while (startIndex < text.length() && text[startIndex] == ' ') {
        startIndex++;
    }
}

void cleanSentence(std::string &sentence) {
    char lettersToRemove[]{
        ',',
    };
    std::transform(sentence.begin(), sentence.end(), sentence.begin(), [](unsigned char c) { return std::tolower(c); });
    for (const auto &l : lettersToRemove) {
        sentence.erase(std::remove(sentence.begin(), sentence.end(), l), sentence.end());
    }
    if (sentence[sentence.length() - 1] == ' ')
        sentence.pop_back();
}

} // namespace utils
