#pragma once

#include "BaseModel.h"
#include <iostream>
#include <string>
#include <utility>

namespace models {
class GeminiTwoZeroFlash : public BaseModel {

  public:
    GeminiTwoZeroFlash(std::string id, std::string url, std::string key, int priority, int rateLimitPerMinute,
                       int rateLimitPerDay)
        : BaseModel(id, url, key, priority, rateLimitPerMinute, rateLimitPerDay) {}

    std::string getId() const override;
    std::string getUrl() const override;
    std::string getKey() const override;
    int getPriority() const override;
    int getRateLimitPerMinute() const override;
    int getRateLimitPerDay() const override;

    std::pair<long, std::string> processPrompt(const network::ClientRequest &request) const override;
};
} // namespace models
