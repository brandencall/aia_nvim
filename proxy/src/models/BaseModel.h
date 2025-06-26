#pragma once

#include "../database/project_repository.h"
#include <string>
#include <utility>

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

    virtual std::string processClientRequest(const network::ClientRequest &request) const;
    virtual std::pair<long, std::string> processPrompt(const network::ClientRequest &request) const = 0;
};
} // namespace models
