#include "../../src/utils/textrank.h"
#include <gtest/gtest.h>
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
      "My Project",
      "Welcome to My Project!",
      "This project is a simple example showing how to write Markdown.",
      "I am writing more to test.",
      "What happens here.",
      "And here?",
      "Features",
      "Easy to read",
      "Supports italic, bold, and",
      "Code blocks with syntax highlighting",
      "Links and images",
      "Usage",
      "Run the following command:"};

  ASSERT_EQ(result, expected);
}
