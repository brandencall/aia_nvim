#include "BaseModel.h"
#include "GeminiTwoFiveFlash.h"
#include "GeminiTwoZeroFlash.h"
#include <memory>
#include <nlohmann/json.hpp>

std::shared_ptr<models::BaseModel> createEndpointFromJson(const nlohmann::json& j){
    std::string id = j.at("id").get<std::string>();
    std::string url = j.at("url").get<std::string>();
    std::string key = j.at("key").get<std::string>();
    int priority = j.at("priority").get<int>();
    int rateLimitPerMinute = j.at("rate_limit_per_minute").get<int>();
    int rateLimitPerDay = j.at("rate_limit_per_day").get<int>();


    if (id.find("Gemini-2.0-Flash") != std::string::npos) {
        return std::make_shared<models::GeminiTwoZeroFlash>(id, url, key, priority, rateLimitPerMinute, rateLimitPerDay);
    } else if (id.find("Gemini-2.5-Flash") != std::string::npos) {
        return std::make_shared<models::GeminiTwoFiveFlash>(id, url, key, priority, rateLimitPerMinute, rateLimitPerDay);
    }

    throw std::runtime_error("Unknown endpoint type: " + id);
}
