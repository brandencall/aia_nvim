
#include "GeminiTwoFiveFlash.h"
#include "../utils/api_request.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

namespace models {

std::string GeminiTwoFiveFlash::processPrompt(const std::string &prompt) const {
    std::string response = sendRequest(endpoint, prompt);
    if (response.empty()) {
        std::cout << "response from llm is empty :(" << std::endl;
        return response;
    }
    try {
        // TODO: Need to update the current Rates at somepoint
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
