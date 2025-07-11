#pragma once

#include "models/BaseModel.h"
#include <memory>
#include <vector>

class ModelManager {
  public:
    std::vector<std::shared_ptr<models::BaseModel>> models;
    std::vector<std::shared_ptr<models::BaseModel>>::iterator modelItorator;

    ModelManager(std::vector<std::shared_ptr<models::BaseModel>> models);
    virtual ~ModelManager() = default;

    virtual std::shared_ptr<models::BaseModel> getCurrentAvailableModel();
    virtual void setNextModel();

};
