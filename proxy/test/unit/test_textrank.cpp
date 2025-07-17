#include "../../src/utils/textrank/preprocess_text.h"
#include "../../src/utils/textrank/tfidf.h"
#include <cmath>
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
