#include "cosine_similarity.h"
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace utils {

double cosineSimilarity(const std::vector<double> &a, const std::vector<double> &b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("The vector sizes need to be the same size");
    }
    double dot = 0;
    double magA = 0;
    double magB = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        magA += a[i] * a[i];
        magB += b[i] * b[i];
    }

    return dot / (std::sqrt(magA) * std::sqrt(magB));
}

} // namespace utils
