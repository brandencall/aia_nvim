#include "../src/ModelManager.h"
#include "test_utils/MockBaseModel.h"
#include <gtest/gtest.h>

TEST(ModelManagerTest, ConstructorModelSort) {
  auto model1 = std::make_shared<MockBaseModel>("model1", "", "", 5, 100, 100);
  auto model2 = std::make_shared<MockBaseModel>("model2", "", "", 2, 100, 100);
  auto model3 = std::make_shared<MockBaseModel>("model3", "", "", 3, 100, 100);

  std::vector<std::shared_ptr<models::BaseModel>> models;
  models.push_back(model1);
  models.push_back(model2);
  models.push_back(model3);
  ModelManager manager(models);

  ASSERT_EQ(manager.models[0]->getId(), "model2");
  ASSERT_EQ(manager.models[1]->getId(), "model3");
  ASSERT_EQ(manager.models[2]->getId(), "model1");

  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model2");
}

TEST(ModelManagerTest, SetNextModel) {
  auto model1 = std::make_shared<MockBaseModel>("model1", "", "", 1, 100, 100);
  auto model2 = std::make_shared<MockBaseModel>("model2", "", "", 2, 100, 100);
  auto model3 = std::make_shared<MockBaseModel>("model3", "", "", 3, 100, 100);

  std::vector<std::shared_ptr<models::BaseModel>> models;
  models.push_back(model1);
  models.push_back(model2);
  models.push_back(model3);
  ModelManager manager(models);

  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model1");
  manager.setNextModel();
  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model2");
  manager.setNextModel();
  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model3");
  manager.setNextModel();
}
