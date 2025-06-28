
#include "../src/ModelManager.h"
#include "MockBaseModel.h"
#include <iostream>
#include <memory>
#include <vector>

class MockModelManager : public ModelManager {
public:
  MockModelManager(
      const std::vector<std::shared_ptr<models::BaseModel>> &models)
      : ModelManager(models) {}
  int currentIndex = 0;
  int modelSwitches = 0;

  std::shared_ptr<models::BaseModel> getCurrentAvailableModel() {
    std::cout << "activeModel: " << activeModel->getId();
    return activeModel;
  }

  void setNextModel() {
    modelSwitches++;
    currentIndex = (currentIndex + 1) % models.size();
    activeModel = models[currentIndex];
  }
};
