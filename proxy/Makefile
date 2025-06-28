CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -I/usr/include/gtest
LDFLAGS = -lgmock -lgtest -lgtest_main -pthread -lcurl -lsqlite3

SRC_DIR = src
TEST_DIR = test
INCLUDE_DIR = -I/usr/include 
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/proxy
TEST_TARGET = $(BIN_DIR)/test

# Find all source files, excluding test directory
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TEST_SOURCES = $(shell find $(TEST_DIR) -name '*.cpp')
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/test/%.o)

# Combine objects for test target (exclude main.o, include shared objects)
SHARED_OBJECTS = $(filter-out $(OBJ_DIR)/main.o,$(OBJECTS))
TEST_ALL_OBJECTS = $(TEST_OBJECTS) $(SHARED_OBJECTS)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(TEST_TARGET): $(TEST_ALL_OBJECTS) | $(BIN_DIR)
	$(CXX) $(TEST_ALL_OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/* $(TARGET) $(TEST_TARGET)

.PHONY: all clean
