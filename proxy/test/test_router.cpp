#include "test_utils/MockRouter.h"
#include <gtest/gtest.h>
#include <memory.h>
#include <memory>
#include <vector>

TEST(RouterTest, Route200Request) {
  auto model1 = std::make_shared<MockBaseModel>("model1", "", "1", 1, 100, 100);
  EXPECT_CALL(*model1, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(200, "")));

  auto model2 = std::make_shared<MockBaseModel>("model2", "", "2", 1, 100, 100);

  std::vector<std::shared_ptr<models::BaseModel>> models;
  models.push_back(model1);
  models.push_back(model2);
  MockModelManager manager(models);

  MockRouter router(1, manager);
  network::HarpoonFile harpoonFile{"", {""}};
  network::Content content{"", {harpoonFile}, "", ""};
  network::ClientRequest request{"prompt", "", content};

  router.handlePromptRequest(request);

  ASSERT_EQ(manager.modelSwitches, 0);
  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model1");
}

TEST(RouterTest, SwitchModelOn404) {
  auto model1 = std::make_shared<MockBaseModel>("model1", "", "1", 1, 100, 100);
  EXPECT_CALL(*model1, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(404, "")));

  auto model2 = std::make_shared<MockBaseModel>("model2", "", "2", 1, 100, 100);
  EXPECT_CALL(*model2, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(200, "OK")));

  std::vector<std::shared_ptr<models::BaseModel>> models;
  models.push_back(model1);
  models.push_back(model2);
  MockModelManager manager(models);

  MockRouter router(1, manager);
  network::HarpoonFile harpoonFile{"", {""}};
  network::Content content{"", {harpoonFile}, "", ""};
  network::ClientRequest request{"prompt", "", content};

  router.handlePromptRequest(request);

  ASSERT_EQ(manager.modelSwitches, 1);
  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model2");
}

TEST(RouterTest, SwitchModelOn429) {
  auto model1 = std::make_shared<MockBaseModel>("model1", "", "1", 1, 100, 100);
  EXPECT_CALL(*model1, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(429, "")));

  auto model2 = std::make_shared<MockBaseModel>("model2", "", "2", 1, 100, 100);
  EXPECT_CALL(*model2, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(200, "OK")));

  std::vector<std::shared_ptr<models::BaseModel>> models;
  models.push_back(model1);
  models.push_back(model2);
  MockModelManager manager(models);

  MockRouter router(1, manager);
  network::HarpoonFile harpoonFile{"", {""}};
  network::Content content{"", {harpoonFile}, "", ""};
  network::ClientRequest request{"prompt", "", content};

  router.handlePromptRequest(request);

  ASSERT_EQ(manager.modelSwitches, 1);
  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model2");
}

TEST(RouterTest, SwitchModelOn509) {
  auto model1 = std::make_shared<MockBaseModel>("model1", "", "1", 1, 100, 100);
  EXPECT_CALL(*model1, processPrompt(::testing::_))
      .Times(3)
      .WillRepeatedly(::testing::Return(std::make_pair(509, "")));

  auto model2 = std::make_shared<MockBaseModel>("model2", "", "2", 1, 100, 100);
  EXPECT_CALL(*model2, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(200, "OK")));

  std::vector<std::shared_ptr<models::BaseModel>> models;
  models.push_back(model1);
  models.push_back(model2);
  MockModelManager manager(models);

  MockRouter router(1, manager);
  network::HarpoonFile harpoonFile{"", {""}};
  network::Content content{"", {harpoonFile}, "", ""};
  network::ClientRequest request{"prompt", "", content};

  router.handlePromptRequest(request);

  ASSERT_EQ(manager.modelSwitches, 1);
  ASSERT_EQ(manager.getCurrentAvailableModel()->getId(), "model2");
}
