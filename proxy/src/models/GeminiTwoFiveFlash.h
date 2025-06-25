#pragma once

#include "BaseModel.h"
#include <iostream>
#include <string>

namespace models {
class GeminiTwoFiveFlash : public BaseModel {
  private:
    std::string _id;
    std::string _url;
    std::string _key;
    int _priority;
    int _rateLimitPerMinute;
    int _rateLimitPerDay;

  public:
    GeminiTwoFiveFlash(std::string id, std::string url, std::string key, int priority, int rateLimitPerMinute,
                       int rateLimitPerDay)
        : _id(id), _url(url), _key(key), _priority(priority), _rateLimitPerMinute(rateLimitPerMinute),
          _rateLimitPerDay(rateLimitPerDay) {}

    std::string getId() const override;
    std::string getUrl() const override;
    std::string getKey() const override;
    int getPriority() const override;
    int getRateLimitPerMinute() const override;
    int getRateLimitPerDay() const override;

    std::pair<long, std::string> processPrompt(const std::string &prompt) const override;
};
} // namespace models
