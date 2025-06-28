#include "../src/models/BaseModel.h"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockBaseModel : public models::BaseModel {

public:
    MockBaseModel(std::string id, std::string url, std::string key, int priority, int rateLimitPerMinute,
                       int rateLimitPerDay)
        : BaseModel(id, url, key, priority, rateLimitPerMinute, rateLimitPerDay) {}

  std::string getId() const override { return _id; }
  std::string getUrl() const override { return _url; }
  std::string getKey() const override { return _key; }
  int getPriority() const override { return _priority; }
  int getRateLimitPerMinute() const override { return _rateLimitPerMinute; }
  int getRateLimitPerDay() const override { return _rateLimitPerDay; }
  MOCK_METHOD((std::pair<long, std::string>), processPrompt,
              (const network::ClientRequest &), (const, override));
};
