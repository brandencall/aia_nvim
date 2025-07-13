#include "../../src/ModelManager.h"
#include "../../src/network/router.h"
#include "../../src/utils/config_loader.h"
#include "../test_utils/MockBaseModel.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <memory.h>
#include <memory>
#include <vector>

TEST(ModelFailerTest, ModelSwitchIntegrationTest) {
  std::filesystem::create_directories("data");
  database::initializeDB();

  std::vector<std::shared_ptr<models::BaseModel>> realModels =
      loadModelsFromConfig(
          "/home/brabs/Projects/plugins/aia.nvim/proxy/endpoints.json");

  auto mockModel =
      std::make_shared<MockBaseModel>("model1", "", "1", 0, 100, 100);
  EXPECT_CALL(*mockModel, processPrompt(::testing::_))
      .WillOnce(::testing::Return(std::make_pair(404, "")));

  std::vector<std::shared_ptr<models::BaseModel>> models = {mockModel,
                                                            realModels[0]};
  ModelManager modelManager(models);

  std::vector<network::HarpoonFile> harpoonFiles;
  network::ClientRequest clientRequest = {
      "prompt", "",
      network::Content{"Just say Hello World", harpoonFiles, "", ""}};

  network::Router *router = new network::Router(-1, modelManager);
  router->routeRequest(clientRequest);

  ASSERT_EQ(modelManager.getCurrentAvailableModel()->getId(),
            realModels[0]->getId());
  EXPECT_NO_THROW({ std::filesystem::remove_all("data"); });
}

