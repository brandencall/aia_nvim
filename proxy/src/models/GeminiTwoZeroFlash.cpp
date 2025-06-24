#include "GeminiTwoZeroFlash.h"
#include "../utils/api_request.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace models {

std::string GeminiTwoZeroFlash::getId() const { return _id; }
std::string GeminiTwoZeroFlash::getUrl() const { return _url; }
std::string GeminiTwoZeroFlash::getKey() const { return _key; }
int GeminiTwoZeroFlash::getPriority() const { return _priority; }
int GeminiTwoZeroFlash::getRateLimitPerMinute() const { return _rateLimitPerMinute; }
int GeminiTwoZeroFlash::getRateLimitPerDay() const { return _rateLimitPerDay; }

std::string GeminiTwoZeroFlash::processPrompt(const std::string &prompt) const {
    std::string endpoint = _url + _key;
    std::string response = sendRequest(endpoint, prompt);
    if (response.empty()) {
        std::cout << "response from llm is empty :(" << std::endl;
        return response;
    }
    try {
        auto response_json = nlohmann::json::parse(response);
        if (response_json.contains("candidates") && !response_json["candidates"].empty()) {
            auto content = response_json["candidates"][0]["content"]["parts"][0]["text"];
            std::cout << "Generated Text: " << content.get<std::string>() << std::endl;
            return content.get<std::string>();
        } else {
            std::cerr << "No candidates found in response: " << response << std::endl;
        }
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        std::cerr << "Raw response: " << response << std::endl;
        return "";
    }
    return "";
}

} // namespace models
