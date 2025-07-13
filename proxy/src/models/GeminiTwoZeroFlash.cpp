#include "GeminiTwoZeroFlash.h"
#include "../utils/api_request.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

namespace models {

std::string GeminiTwoZeroFlash::getId() const { return _id; }
std::string GeminiTwoZeroFlash::getUrl() const { return _url; }
std::string GeminiTwoZeroFlash::getKey() const { return _key; }
int GeminiTwoZeroFlash::getPriority() const { return _priority; }
int GeminiTwoZeroFlash::getRateLimitPerMinute() const { return _rateLimitPerMinute; }
int GeminiTwoZeroFlash::getRateLimitPerDay() const { return _rateLimitPerDay; }

std::pair<long, std::string> GeminiTwoZeroFlash::processPrompt(const network::ClientRequest& request) const {
    std::string endpoint = _url + _key;
    std::string prompt = processClientRequest(request);
    std::pair<long, std::string> response = utils::sendRequest(endpoint, prompt);
    if (response.second.empty()) {
        std::cout << "response from llm is empty :(" << std::endl;
        return response;
    }
    try {
        auto response_json = nlohmann::json::parse(response.second);
        if (response_json.contains("candidates") && !response_json["candidates"].empty()) {
            auto content = response_json["candidates"][0]["content"]["parts"][0]["text"];
            std::cout << "Generated Text: " << content.get<std::string>() << std::endl;
            return {response.first, content.get<std::string>()};
        } else {
            std::cerr << "No candidates found in response: " << response.second << std::endl;
        }
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        std::cerr << "Raw response: " << response.second << std::endl;
        return response;
    }
    return response;
}
} // namespace models
