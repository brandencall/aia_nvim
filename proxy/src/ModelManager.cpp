#include "ModelManager.h"
#include "models/BaseModel.h"
#include <iostream>
#include <memory>

std::shared_ptr<models::BaseModel> ModelManager::getCurrentAvailableModel() { return activeModel; }

// TODO: this is not correct. Need a better way of finding the next model. Need to keep track which models have been
// tried.
//  may need to set models to active/deactive to make sure that we aren't looping through same bad models
void ModelManager::setNextModel() {
    std::cout << "I shouldn't be using this one!!!" << std::endl;
    int currentModelPriority = activeModel->getPriority();
    for (const auto &model : models) {
        if (currentModelPriority + 1 == model->getPriority()) {
            activeModel = model;
        }
    }
    activeModel = models[0];
}
