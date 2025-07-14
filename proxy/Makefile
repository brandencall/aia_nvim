# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -I/usr/include/gtest
LDFLAGS = -lgmock -lgtest -lgtest_main -pthread -lcurl -lsqlite3 -lcmark

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

UNIT_TEST_DIR = test/unit
INTEGRATION_TEST_DIR = test/integration

# Binaries
TARGET = $(BIN_DIR)/proxy
UNIT_TEST_TARGET = $(BIN_DIR)/test_unit
INTEGRATION_TEST_TARGET = $(BIN_DIR)/test_integration

# Sources
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
SHARED_OBJECTS = $(filter-out $(OBJ_DIR)/main.o,$(OBJECTS))

# Test source files
UNIT_TEST_SOURCES = $(shell find $(UNIT_TEST_DIR) -name '*.cpp')
UNIT_TEST_OBJECTS = $(UNIT_TEST_SOURCES:$(UNIT_TEST_DIR)/%.cpp=$(OBJ_DIR)/test/unit/%.o)

INTEGRATION_TEST_SOURCES = $(shell find $(INTEGRATION_TEST_DIR) -name '*.cpp')
INTEGRATION_TEST_OBJECTS = $(INTEGRATION_TEST_SOURCES:$(INTEGRATION_TEST_DIR)/%.cpp=$(OBJ_DIR)/test/integration/%.o)

# Targets
all: $(TARGET) $(UNIT_TEST_TARGET) $(INTEGRATION_TEST_TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(UNIT_TEST_TARGET): $(UNIT_TEST_OBJECTS) $(SHARED_OBJECTS) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(INTEGRATION_TEST_TARGET): $(INTEGRATION_TEST_OBJECTS) $(SHARED_OBJECTS) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compilation rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test/unit/%.o: $(UNIT_TEST_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test/integration/%.o: $(INTEGRATION_TEST_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Directories
$(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
