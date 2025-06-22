#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <string>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
  CURL *curl;
  CURLcode result;

  std::string response_data;

  curl = curl_easy_init();
  if (curl == NULL) {
    std::cerr << stderr << " HTTP request failed" << std::endl;
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

  result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
    return -1;
  }
  std::cout << response_data << " <----- response string" << std::endl;
  curl_easy_cleanup(curl);

  return 0;
}
