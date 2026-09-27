CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

BIN_DIR = bin

GRAPH_SOURCES = \
	src/core/graph.cpp \
	src/core/graph_generator.cpp

TEST_GRAPH_SOURCE = src/programs/test_small_graph.cpp

TEST_GRAPH = $(BIN_DIR)/test_graph

.PHONY: all clean test

all: $(TEST_GRAPH)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_GRAPH): $(GRAPH_SOURCES) $(TEST_GRAPH_SOURCE) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(GRAPH_SOURCES) $(TEST_GRAPH_SOURCE) -o $(TEST_GRAPH)

test: $(TEST_GRAPH)
	./$(TEST_GRAPH)

clean:
	rm -rf $(BIN_DIR)