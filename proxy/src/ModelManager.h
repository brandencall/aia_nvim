#pragma once

#include "models/BaseModel.h"
#include <memory>
#include <vector>

class ModelManager {
  public:
    ModelManager(const std::vector<std::shared_ptr<models::BaseModel>> &models) : _models(models) {
        _activeModel = models[0];
        for (const auto &model : models) {
            if (model->getPriority() == 1) {
                _activeModel = model;
                break;
            }
        }
    }

    std::shared_ptr<models::BaseModel> getCurrentAvailableModel();
    void setNextModel();

  private:
    std::vector<std::shared_ptr<models::BaseModel>> _models;
    std::shared_ptr<models::BaseModel> _activeModel;
};
