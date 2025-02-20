CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -lgtest -lgtest_main -pthread -lexpat
SRC_DIR := src
TEST_SRC_DIR := testsrc
BIN_DIR := bin
OBJ_DIR := obj

# Ensure directories exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES := $(wildcard $(TEST_SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_FILES))

# Executables
TEST_EXEC_STRUTILS := $(BIN_DIR)/teststrutils
TEST_EXEC_STRDATASINK := $(BIN_DIR)/teststrdatasink
TEST_EXEC_STRDATASOURCE := $(BIN_DIR)/teststrdatasource
TEST_EXEC_DSV := $(BIN_DIR)/testdsv
TEST_EXEC_XML := $(BIN_DIR)/testxml

all: teststrutils teststrdatasink teststrdatasource testdsv testxml run_tests

teststrutils: $(TEST_EXEC_STRUTILS)
teststrdatasink: $(TEST_EXEC_STRDATASINK)
teststrdatasource: $(TEST_EXEC_STRDATASOURCE)
testdsv: $(TEST_EXEC_DSV)
testxml: $(TEST_EXEC_XML)

$(TEST_EXEC_STRUTILS): $(OBJ_DIR)/StringUtils.o $(OBJ_DIR)/StringUtilsTest.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_EXEC_STRDATASINK): $(OBJ_DIR)/StringDataSink.o $(OBJ_DIR)/StringDataSinkTest.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_EXEC_STRDATASOURCE): $(OBJ_DIR)/StringDataSource.o $(OBJ_DIR)/StringDataSourceTest.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_EXEC_DSV): $(OBJ_DIR)/DSVReader.o $(OBJ_DIR)/DSVWriter.o $(OBJ_DIR)/DSVTest.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_EXEC_XML): $(OBJ_DIR)/XMLReader.o $(OBJ_DIR)/XMLWriter.o $(OBJ_DIR)/XMLTest.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run_tests: all
	$(BIN_DIR)/teststrutils
	$(BIN_DIR)/teststrdatasink
	$(BIN_DIR)/teststrdatasource
	$(BIN_DIR)/testdsv
	$(BIN_DIR)/testxml

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean teststrutils teststrdatasink teststrdatasource testdsv testxml run_tests
