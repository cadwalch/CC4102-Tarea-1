CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

BIN_DIR = bin

CORE_SOURCES = \
	src/core/graph.cpp \
	src/core/graph_generator.cpp \
	src/core/binomial_heap.cpp \
	src/core/prim.cpp

TEST_SOURCE = src/programs/test_small_graph.cpp

TEST_BIN = $(BIN_DIR)/test_graph

.PHONY: all test clean

all: $(TEST_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_BIN): $(CORE_SOURCES) $(TEST_SOURCE) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CORE_SOURCES) $(TEST_SOURCE) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BIN_DIR)