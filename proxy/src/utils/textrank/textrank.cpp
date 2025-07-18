#include "textrank.h"
#include "cosine_similarity.h"
#include "preprocess_text.h"
#include "tfidf.h"
#include <algorithm>
#include <cmark.h>
#include <cstddef>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace utils {

std::string summarizeText(const std::vector<std::string> &text) {
    std::string result;

    std::vector<std::string> sentences = cleanText(text);
    tfidf tfidfScores{sentences};
    // 2D matrix of cosine similarity scores. graph[i][j] is cosine similarity score between sentence i and j.
    std::vector<std::vector<double>> graph = buildTextRankGraph(tfidfScores.matrix);
    std::vector<double> scores = textrank(graph);
    std::vector<std::pair<std::string, double>> topSentences = getTopSentences(sentences, scores);

    return concatinateSentences(topSentences);
}

std::vector<std::string> cleanText(const std::vector<std::string> &text) {
    std::string full_text =
        std::accumulate(text.begin(), text.end(), std::string(), [](const std::string &acc, const std::string &line) {
            return acc.empty() ? line : acc + "\n" + line;
        });
    std::string plainText = stripMarkdown(full_text);
    return getSentences(plainText);
}

std::vector<std::vector<double>> buildTextRankGraph(const std::vector<std::vector<double>> &tfidfMatrix) {
    std::vector<std::vector<double>> textrankGraph(tfidfMatrix.size(), std::vector<double>(tfidfMatrix.size(), 0.0));
    for (size_t i = 0; i < tfidfMatrix.size(); ++i) {
        for (size_t j = i + 1; j < tfidfMatrix.size(); ++j) {
            double cosineScore = cosineSimilarity(tfidfMatrix[i], tfidfMatrix[j]);
            textrankGraph[i][j] = cosineScore;
            textrankGraph[j][i] = cosineScore;
        }
    }
    return textrankGraph;
}

// text rank algorithm: https://web.eecs.umich.edu/~mihalcea/papers/mihalcea.emnlp04.pdf
std::vector<double> textrank(const std::vector<std::vector<double>> &graph) {
    const double d = 0.85;
    const size_t maxIterations = 100;
    const double convergenceThreshold = 0.0001;
    size_t numSentences = graph.size();

    std::vector<double> scores(graph.size(), 1.0);

    for (size_t iter = 0; iter < maxIterations; ++iter) {
        std::vector<double> newScores(numSentences, 0.0);
        double maxChange = 0;

        for (size_t i = 0; i < numSentences; ++i) {
            double sum = 0.0;
            for (size_t j = 0; j < numSentences; ++j) {
                if (i == j || graph[j][i] == 0.0)
                    continue;

                double weightSum = 0.0;
                for (size_t k = 0; k < numSentences; ++k)
                    weightSum += graph[j][k];

                if (weightSum != 0)
                    sum += (graph[j][i] / weightSum) * scores[j];
            }
            newScores[i] = (1 - d) + d * sum;
            maxChange = std::max(maxChange, std::abs(newScores[i] - scores[i]));
        }
        scores = newScores;
        if (maxChange < convergenceThreshold)
            break;
    }

    return scores;
}

std::vector<std::pair<std::string, double>> getTopSentences(const std::vector<std::string> &sentences,
                                                            const std::vector<double> &scores, size_t top) {
    std::vector<std::pair<std::string, double>> paired;
    for (size_t i = 0; i < sentences.size(); ++i) {
        paired.emplace_back(sentences[i], scores[i]);
    }
    std::sort(paired.begin(), paired.end(), [](const auto &a, const auto &b) { return a.second > b.second; });
    if (paired.size() > top) {
        paired.resize(top);
    }
    return paired;
}

std::string concatinateSentences(const std::vector<std::pair<std::string, double>> &topSentences) {
    std::ostringstream oss;
    for (const auto &sentence : topSentences) {
        oss << sentence.first << ". ";
    }
    return oss.str();
}

} // namespace utils
