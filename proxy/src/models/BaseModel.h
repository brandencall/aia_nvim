#pragma once

#include <string>
namespace models {
class BaseModel {
  public:

    virtual ~BaseModel() = default; 

    virtual std::string getId() const = 0;
    virtual std::string getUrl() const = 0;
    virtual std::string getKey() const = 0;
    virtual int getPriority() const = 0;
    virtual int getRateLimitPerMinute() const = 0;
    virtual int getRateLimitPerDay() const = 0;

    virtual std::string processPrompt(const std::string &prompt) const = 0;
};
} // namespace models
