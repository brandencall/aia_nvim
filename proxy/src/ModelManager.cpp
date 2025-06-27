#include "ModelManager.h"
#include "models/BaseModel.h"
#include <memory>

std::shared_ptr<models::BaseModel> ModelManager::getCurrentAvailableModel() { return _activeModel; }

// TODO: this is not correct. Need a better way of finding the next model. Need to keep track which models have been
// tried.
//  may need to set models to active/deactive to make sure that we aren't looping through same bad models
void ModelManager::setNextModel() {
    int currentModelPriority = _activeModel->getPriority();
    for (const auto &model : _models) {
        if (currentModelPriority + 1 == model->getPriority()) {
            _activeModel = model;
        }
    }
    _activeModel = _models[0];
}
