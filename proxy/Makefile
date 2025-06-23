CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra 
LDFLAGS = -lgtest -lgtest_main -pthread -lcurl

SRC_DIR = src
INCLUDE_DIR = -I/usr/include 
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/proxy

# Find all source files, excluding test directory
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Combine objects for test target (exclude main.o, include shared objects)
SHARED_OBJECTS = $(filter-out $(OBJ_DIR)/main.o,$(OBJECTS))

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/* $(TARGET) $(TEST_TARGET)

.PHONY: all clean
