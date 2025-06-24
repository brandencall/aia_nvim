#pragma once

#include "BaseModel.h"
#include <iostream>
#include <string>

namespace models {
class GeminiTwoZeroFlash : public BaseModel {
  private:
    std::string _id;
    std::string _url;
    std::string _key;
    int _priority;
    int _rateLimitPerMinute;
    int _rateLimitPerDay;

  public:
    GeminiTwoZeroFlash(std::string id, std::string url, std::string key, int priority, int rateLimitPerMinute,
                       int rateLimitPerDay)
        : _id(id), _url(url), _key(key), _priority(priority), _rateLimitPerMinute(rateLimitPerMinute),
          _rateLimitPerDay(rateLimitPerDay) {}

    std::string getId() const override;
    virtual std::string getUrl() const override;
    virtual std::string getKey() const override;
    virtual int getPriority() const override;
    virtual int getRateLimitPerMinute() const override;
    virtual int getRateLimitPerDay() const override;

    std::string processPrompt(const std::string &prompt) const override;
};
} // namespace models
