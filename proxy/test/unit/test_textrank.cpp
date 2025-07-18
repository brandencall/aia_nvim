#include "../../src/utils/textrank/cosine_similarity.h"
#include "../../src/utils/textrank/preprocess_text.h"
#include "../../src/utils/textrank/tfidf.h"
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

TEST(TextRankTest, StripMarkdown) {
  std::string markdown = R"(
# My Project

Welcome to **My Project**! This project is a simple example showing how to write Markdown.

## Features

- Easy to read
- Supports _italic_, **bold**, and `inline code`
- Code blocks with syntax highlighting
- Links and images

## Usage

Run the following command:

```bash
./run_project --help
```
)";
  std::string expected = R"(My Project
Welcome to My Project! This project is a simple example showing how to write Markdown.
Features
Easy to read
Supports italic, bold, and 
Code blocks with syntax highlighting
Links and images
Usage
Run the following command:)";

  std::string result = utils::stripMarkdown(markdown);
  ASSERT_EQ(result, expected);
}

TEST(TextRankTest, GetSentences) {
  std::string test = R"(My Project
Welcome to My Project! This project is a simple example showing how to write Markdown. I am writing more to test.
What happens here.      And here?
Features
Easy to read
Supports italic, bold, and 
Code blocks with syntax highlighting
Links and images
Usage
Run the following command:)";
  std::vector<std::string> result = utils::getSentences(test);

  std::vector<std::string> expected = {
      "my project",
      "welcome to my project",
      "this project is a simple example showing how to write markdown",
      "i am writing more to test",
      "what happens here",
      "and here",
      "features",
      "easy to read",
      "supports italic bold and",
      "code blocks with syntax highlighting",
      "links and images",
      "usage",
      "run the following command"};

  ASSERT_EQ(result, expected);
}

TEST(TextRankTest, ComputeTFIDF) {
  std::vector<std::string> test = {
      "the cat sat on the mat",
      "the dog played in the park",
      "cats and dogs are great pets",
  };
  // Only building the expected result for cat
  std::string word = "cat";
  double value = 0.029348543175946873;

  utils::tfidf tfidf{test};

  int catIndex = tfidf.getIndexFromWord(word);
  ASSERT_DOUBLE_EQ(tfidf.matrix[0][catIndex], value);
  ASSERT_DOUBLE_EQ(tfidf.matrix[1][catIndex], 0);
  ASSERT_DOUBLE_EQ(tfidf.matrix[2][catIndex], 0);
}

TEST(TextRankTest, ComputeCosineSimilarity) {
  std::vector<std::string> test = {
      "the cat sat on the mat", "the dog sat on the log",
      "the cat chased the mouse", "the dog chased the cat"};

  // Similarity between sentence 1 and 2: 0.30427
  // Similarity between sentence 1 and 3: 0.0304869
  // Similarity between sentence 1 and 4: 0.0470432
  // Similarity between sentence 2 and 3: 0
  // Similarity between sentence 2 and 4: 0.256446
  // Similarity between sentence 3 and 4: 0.349725
  std::vector<double> expected = {
      0.30426990995437397,
      0.030486859851721525,
      0.047043236224699983,
      0,
      0.25644572558810497,
      0.34972524594261745,
  };

  int expectedCount = 0;
  utils::tfidf tfidf{test};

  for (size_t i = 0; i < tfidf.matrix.size(); ++i) {
    for (size_t j = i + 1; j < tfidf.matrix.size(); ++j) {
      double similarity =
          utils::cosineSimilarity(tfidf.matrix[i], tfidf.matrix[j]);
      ASSERT_DOUBLE_EQ(similarity, expected[expectedCount]);
      ++expectedCount;
    }
  }
}
