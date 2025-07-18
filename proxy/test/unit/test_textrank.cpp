#include "../../src/utils/textrank/cosine_similarity.h"
#include "../../src/utils/textrank/preprocess_text.h"
#include "../../src/utils/textrank/textrank.h"
#include "../../src/utils/textrank/tfidf.h"
#include <cstddef>
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
      0.30426990995437397, 0.030486859851721525, 0.047043236224699983, 0,
      0.25644572558810497, 0.34972524594261745,
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

TEST(TextRankTest, TextRankGraph) {
  std::vector<std::string> test = {
      "the cat sat on the mat", "the dog sat on the log",
      "the cat chased the mouse", "the dog chased the cat"};

  // 0, 0.30427, 0.0304869, 0.0470432,
  // 0.30427, 0, 0, 0.256446,
  // 0.0304869, 0, 0, 0.349725,
  // 0.0470432, 0.256446, 0.349725, 0,
  std::vector<std::vector<double>> expected = {
      {0, 0.30426990995437397, 0.030486859851721525, 0.047043236224699983},
      {0.30426990995437397, 0, 0, 0.25644572558810497},
      {0.030486859851721525, 0, 0, 0.34972524594261745},
      {0.047043236224699983, 0.25644572558810497, 0.34972524594261745, 0}};
  utils::tfidf tfidf{test};

  auto graph = utils::buildTextRankGraph(tfidf.matrix);

  for (size_t i = 0; i < graph.size(); ++i) {
    for (size_t j = 0; j < graph.size(); ++j) {
      ASSERT_DOUBLE_EQ(graph[i][j], expected[i][j]);
    }
  }
}
TEST(TextRankTest, SummarizeText) {
  std::vector<std::string> test = {
      "What's the weather like today in New York?",
      "The weather in New York is currently 75°F with partly cloudy skies.",
      "Great, should I bring an umbrella?",
      ("There's a 20% chance of rain in the afternoon. It might be a good idea "
       "to bring a small umbrella just in case."),
      "Thanks! Can you also remind me what meetings I have today?",
      ("Sure. You have a team sync at 10:00 AM, a client call at 1:00 PM, and "
       "a "
       "project review at 3:30 PM."),
      "Can you move the project review to tomorrow afternoon?",
      ("I've moved your project review to 2:00 PM tomorrow. Would you like me "
       "to notify the team?"),
      "Yes, please do that.",
      "Done. The team has been notified of the schedule change."};

  std::string expected =
      "can you move the project review to tomorrow afternoon. what's the "
      "weather like today in new york. would you like me to notify the team. "
      "i've moved your project review to 200 pm tomorrow. you have a team sync "
      "at 1000 am a client call at 100 pm and a project review at 330 pm. ";

  std::string result = utils::summarizeText(test);

  ASSERT_EQ(result, expected);
}
