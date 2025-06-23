#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

void loadEnvFile(const std::string &filePath) {
  std::ifstream envFile(filePath);
  if (!envFile.is_open()) {
    std::cout << "couldn't open up the file" << std::endl;
    return;
  }

  std::string line;
  while (std::getline(envFile, line)) {
    if (line.empty() || line[0] == '#')
      continue;

    size_t delimPos = line.find('=');
    if (delimPos == std::string::npos)
      continue;

    std::string key = line.substr(0, delimPos);
    std::string value = line.substr(delimPos + 1);

    setenv(key.c_str(), value.c_str(), 1);
  }
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  std::string *userp) {
  userp->append((char *)contents, size * nmemb);
  return size * nmemb;
}

int main() {
  loadEnvFile(".env");
  const char *key = std::getenv("GEMINI_KEY");
  const char *url = std::getenv("GEMINI_2_0_FLASH_URL");
  if (!key || !url) {
    std::cerr << "key or url are empty" << std::endl;
    return 1;
  }
  std::string fullURL(url);
  fullURL += std::string(key);

  CURL *curl;
  CURLcode result;

  std::string response_data;

  curl = curl_easy_init();
  if (curl == NULL) {
    std::cerr << stderr << " HTTP request failed" << std::endl;
    return -1;
  }

  nlohmann::json request_body;
  request_body["contents"] = nlohmann::json::array();
  request_body["contents"].push_back(
      {{"parts", nlohmann::json::array(
                     {{{"text", "Explain how AI works in a few words"}}})}});
  std::string request_body_str = request_body.dump();

  curl_easy_setopt(curl, CURLOPT_URL, fullURL.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body_str.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_body_str.length());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
    return -1;
  }

  // Check HTTP status code
  long http_code = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
  if (http_code != 200) {
    std::cerr << "HTTP request failed with status code: " << http_code
              << std::endl;
    std::cerr << "Response: " << response_data << std::endl;
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return 1;
  }

  // Clean up curl
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  // Parse the response JSON
  try {
    auto response_json = nlohmann::json::parse(response_data);
    // Extract the generated text (adjust path based on actual response
    // structure)
    if (response_json.contains("candidates") &&
        !response_json["candidates"].empty()) {
      auto content =
          response_json["candidates"][0]["content"]["parts"][0]["text"];
      std::cout << "Generated Text: " << content.get<std::string>()
                << std::endl;
    } else {
      std::cerr << "No candidates found in response: " << response_data
                << std::endl;
    }
    if (response_json.contains("usageMetadata") &&
        !response_json["usageMetadata"].empty()) {
      auto totalTokenCount = response_json["usageMetadata"]["totalTokenCount"];
      auto promptToken = response_json["usageMetadata"]["promptTokenCount"];
      auto candidateToken = response_json["usageMetadata"]["candidatesTokenCount"];
      std::cout << "totalTokenCount: " << totalTokenCount.get<int>()
                << std::endl;
      std::cout << "promptCount: " << promptToken.get<int>()
                << std::endl;
      std::cout << "candidate: " << candidateToken.get<int>()
                << std::endl;
    }
    std::cout << response_json;
  } catch (const nlohmann::json::exception &e) {
    std::cerr << "JSON parsing error: " << e.what() << std::endl;
    std::cerr << "Raw response: " << response_data << std::endl;
    return 1;
  }

  return 0;
}
