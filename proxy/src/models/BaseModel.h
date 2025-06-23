#pragma once

#include <string>
namespace models {
class BaseModel {
  public:
    int currentRPM = 0;
    int currentRPD = 0;
    long currentTPM = 0;
    int limitRPM = 0;
    int limitRPD = 0;
    long limitTPM = 0;

    virtual ~BaseModel() {}
    virtual std::string processPrompt(const std::string &prompt) const = 0;
};
} // namespace models
