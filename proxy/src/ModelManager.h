#pragma once

#include "models/BaseModel.h"
#include <memory>
#include <vector>

class ModelManager {
  public:
    static void init(const std::vector<std::shared_ptr<models::BaseModel>> &models);
    static ModelManager &getInstance();

    std::shared_ptr<models::BaseModel> getNextAvailableModel();
    std::shared_ptr<models::BaseModel> getCurrentAvailableModel();

  private:
    ModelManager(const std::vector<std::shared_ptr<models::BaseModel>> &models);
    std::vector<std::shared_ptr<models::BaseModel>> _models;
    static bool _initialized;
    std::shared_ptr<models::BaseModel> _activeModel;
};
