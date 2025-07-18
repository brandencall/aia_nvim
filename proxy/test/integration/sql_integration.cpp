#include "../../src/network/router.h"
#include "../test_utils/MockBaseModel.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <memory.h>
#include <string>

TEST(SQLiteIntegration, GetRecentConversations) {
  std::filesystem::create_directories("data");
  database::initializeDB();

  database::Project project = {1, "1", "", ""};
  std::vector<network::HarpoonFile> harpoonFiles;
  network::ClientRequest clientRequest = {
      "prompt", project.project_id,
      network::Content{"Just say Hello World", harpoonFiles, "", ""}};
  std::string response = "Hello World";

  database::insertProject(clientRequest);
  database::insertChat(clientRequest, response);
  std::vector<std::string> result = database::getRecentConversations(clientRequest);
  ASSERT_EQ(result[0], "Just say Hello World Hello World");
  EXPECT_NO_THROW({ std::filesystem::remove_all("data"); });
}
