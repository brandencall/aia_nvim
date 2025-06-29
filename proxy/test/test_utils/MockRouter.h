#include "../../src/network/router.h"
#include "MockModelManager.h"

class MockRouter : public network::Router {
public:
  using network::Router::handlePromptRequest;
  using network::Router::handleResponse;

  MockRouter(int socket, MockModelManager &mm) : Router(socket, mm) {}
};
