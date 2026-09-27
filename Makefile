CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

BIN_DIR = bin

CORE_SOURCES = \
	src/core/graph.cpp \
	src/core/graph_generator.cpp \
	src/core/binomial_heap.cpp \
	src/core/fibonacci_heap.cpp \
	src/core/prim.cpp \
	src/utils/experiment_utils.cpp

MAIN_SOURCE = src/programs/main.cpp
MAIN_BIN = $(BIN_DIR)/main

.PHONY: all small small-amortized experiments amortized plots clean clean-results clean-all

all: $(MAIN_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(MAIN_BIN): $(CORE_SOURCES) $(MAIN_SOURCE) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(CORE_SOURCES) $(MAIN_SOURCE) -o $(MAIN_BIN)

small: $(MAIN_BIN)
	mkdir -p results/raw
	./$(MAIN_BIN) --small

small-amortized: $(MAIN_BIN)
	mkdir -p results/raw
	./$(MAIN_BIN) --small-amortized

experiments: $(MAIN_BIN)
	mkdir -p results/raw
	./$(MAIN_BIN) --all

amortized: $(MAIN_BIN)
	mkdir -p results/raw
	./$(MAIN_BIN) --amortized

plots:
	python3 scripts/plot_results.py

clean:
	rm -rf $(BIN_DIR)

clean-results:
	rm -f results/raw/*.csv
	rm -f results/plots/*.pdf

clean-all: clean clean-results