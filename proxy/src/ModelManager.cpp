#include "ModelManager.h"
#include "models/BaseModel.h"
#include <algorithm>
#include <memory>
#include <nlohmann/detail/value_t.hpp>

ModelManager::ModelManager(std::vector<std::shared_ptr<models::BaseModel>> inputModels) {
    std::sort(inputModels.begin(), inputModels.end(),
              [](const auto &a, const auto &b) { return a->getPriority() < b->getPriority(); });
    models = inputModels;
    modelItorator = models.begin();
}

std::shared_ptr<models::BaseModel> ModelManager::getCurrentAvailableModel() { return *modelItorator; }

// TODO: this is not correct. Need a better way of finding the next model. Need to keep track which models have been
// tried.
//  may need to set models to active/deactive to make sure that we aren't looping through same bad models
void ModelManager::setNextModel() {
    ++modelItorator;
    if (modelItorator == models.end()) {
        modelItorator = models.begin();
    }
}
