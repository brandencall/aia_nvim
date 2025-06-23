#pragma once

#include "BaseModel.h"
#include <iostream>
#include <string>

namespace models {
class GeminiTwoFiveFlash : public BaseModel {
  private:
    std::string endpoint = "";

  public:
    GeminiTwoFiveFlash() {
        limitRPM = 10;
        limitRPD = 500;
        limitTPM = 1000000;
        const char *url = std::getenv("GEMINI_2_5_FLASH_URL");
        const char *key = std::getenv("GEMINI_KEY");
        if (!key || !url) {
            std::cerr << "key or url are empty" << std::endl;
        }
        endpoint += std::string(url);
        endpoint += std::string(key);
    }

    std::string processPrompt(const std::string &prompt) const override;
};
} // namespace models
