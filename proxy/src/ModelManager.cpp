#include "ModelManager.h"
#include "models/BaseModel.h"
#include <iostream>
#include <memory>

bool ModelManager::_initialized = false;
static ModelManager *instance = nullptr;

ModelManager::ModelManager(const std::vector<std::shared_ptr<models::BaseModel>> &models) : _models(models) {
    _activeModel = models[0];
    for (const auto &model : models) {
        if (model->getPriority() == 1) {
            _activeModel = model;
            break;
        }
    }
}

void ModelManager::init(const std::vector<std::shared_ptr<models::BaseModel>> &models) {
    if (!_initialized) {
        instance = new ModelManager(models);
        _initialized = true;
    } else {
        throw std::runtime_error("ModelManager already initialized");
    }
}

ModelManager &ModelManager::getInstance() {
    if (!_initialized) {
        throw std::runtime_error("ModelManager not initialized. Call Init() first.");
    }
    return *instance;
}

std::shared_ptr<models::BaseModel> ModelManager::getCurrentAvailableModel() { return _activeModel; }

//TODO: this is not correct. Need a better way of finding the next model. Need to keep track which models have been tried.
// may need to set models to active/deactive to make sure that we aren't looping through same bad models
void ModelManager::setNextModel(){
    int currentModelPriority = _activeModel->getPriority();
    for (const auto& model : _models){
        if(currentModelPriority + 1 == model->getPriority()){
            _activeModel = model;
        }
    }
    _activeModel = _models[0];
}
