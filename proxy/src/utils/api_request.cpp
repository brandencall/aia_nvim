#include <curl/curl.h>
#include <curl/easy.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

namespace utils {

static size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    userp->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::pair<long, std::string> sendRequest(const std::string &endpoint, const std::string &prompt) {
    CURL *curl;
    CURLcode result;

    std::string response_data;

    curl = curl_easy_init();
    if (curl == NULL) {
        std::cerr << stderr << " HTTP request failed" << std::endl;
        return {-1, ""};
    }
    nlohmann::json request_body;
    request_body["contents"] = nlohmann::json::array();
    request_body["contents"].push_back({{"parts", nlohmann::json::array({{{"text", prompt}}})}});
    std::string request_body_str = request_body.dump();

    curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body_str.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request_body_str.length());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    result = curl_easy_perform(curl);
    if (result != CURLE_OK) {
        std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
        return {-1, ""};
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        std::cerr << "HTTP request failed with status code: " << http_code << std::endl;
        std::cerr << "Response: " << response_data << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return {http_code, ""};
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return {http_code, response_data};
}

} // namespace utils
