#pragma once

#include "models/BaseModel.h"
#include <memory>
#include <vector>

class ModelManager {
  public:
    std::vector<std::shared_ptr<models::BaseModel>> models;
    std::shared_ptr<models::BaseModel> activeModel;

    ModelManager(const std::vector<std::shared_ptr<models::BaseModel>> &models)
        : models(models), activeModel(models[0]) {
        for (const auto &model : models) {
            if (model->getPriority() == 1) {
                activeModel = model;
                break;
            }
        }
    }
    virtual ~ModelManager() = default;
    virtual std::shared_ptr<models::BaseModel> getCurrentAvailableModel();
    virtual void setNextModel();
};
