#pragma once

#include "../database/project_repository.h"
#include "../database/summary_repository.h"
#include <string>
#include <utility>

namespace models {
class BaseModel {
  protected:
    std::string _id;
    std::string _url;
    std::string _key;
    int _priority;
    int _rateLimitPerMinute;
    int _rateLimitPerDay;

    std::string processClientContent(const std::optional<network::Content> &content) const;
    std::string processHarpoonContent(const std::optional<network::Content> &content) const;

  public:
    BaseModel(std::string id, std::string url, std::string key, int priority, int rateLimitPerMinute,
              int rateLimitPerDay)
        : _id(id), _url(url), _key(key), _priority(priority), _rateLimitPerMinute(rateLimitPerMinute),
          _rateLimitPerDay(rateLimitPerDay) {}

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
