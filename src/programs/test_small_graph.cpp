#include "binomial_heap.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

bool is_connected(const Graph& graph) {
    if (graph.vertex_count() == 0) {
        return true;
    }

    std::vector<bool> visited(graph.vertex_count(), false);
    std::queue<int> pending;

    visited[0] = true;
    pending.push(0);

    int visited_count = 0;

    while (!pending.empty()) {
        int vertex = pending.front();
        pending.pop();

        visited_count++;

        for (const Edge& edge : graph.neighbors(vertex)) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                pending.push(edge.to);
            }
        }
    }

    return visited_count == graph.vertex_count();
}

bool is_simple(const Graph& graph) {
    std::set<std::pair<int, int>> edges;

    for (int u = 0; u < graph.vertex_count(); ++u) {
        for (const Edge& edge : graph.neighbors(u)) {
            int v = edge.to;

            if (u == v) {
                return false;
            }

            if (u < v) {
                if (!edges.insert({u, v}).second) {
                    return false;
                }
            }
        }
    }

    return true;
}

void test_graph_generator() {
    const int n = 10;
    const int m = 15;
    const unsigned int seed = 42;

    Graph graph =
        GraphGenerator::generate_connected_graph(n, m, seed);

    assert(graph.vertex_count() == n);
    assert(graph.edge_count() == m);
    assert(is_connected(graph));
    assert(is_simple(graph));

    std::cout << "Graph generator: OK" << std::endl;
}

void test_graph_generator_large() {
    const int n = 10000;
    const int m = 50000;
    const unsigned int seed = 12345;

    Graph graph =
        GraphGenerator::generate_connected_graph(n, m, seed);

    assert(graph.vertex_count() == n);
    assert(graph.edge_count() == m);
    assert(is_connected(graph));
    assert(is_simple(graph));

    std::cout << "Graph generator large: OK" << std::endl;
}

void test_binomial_extract_min() {
    std::vector<double> keys = {
        5.0, // vertex 0
        2.0, // vertex 1
        8.0, // vertex 2
        1.0, // vertex 3
        4.0  // vertex 4
    };

    BinomialHeap heap;
    heap.build(keys);

    assert(!heap.empty());

    assert(heap.extract_min() == 3);
    assert(heap.extract_min() == 1);
    assert(heap.extract_min() == 4);
    assert(heap.extract_min() == 0);
    assert(heap.extract_min() == 2);

    assert(heap.empty());

    std::cout << "Binomial extract_min: OK" << std::endl;
}

void test_binomial_multiple_sizes() {
    for (int n = 1; n <= 100; ++n) {
        std::vector<double> keys;

        for (int i = 0; i < n; ++i) {
            keys.push_back(static_cast<double>(n - i));
        }

        BinomialHeap heap;
        heap.build(keys);

        for (int expected_vertex = n - 1;
             expected_vertex >= 0;
             --expected_vertex) {
            int extracted = heap.extract_min();
            assert(extracted == expected_vertex);
        }

        assert(heap.empty());
    }

    std::cout << "Binomial multiple sizes: OK" << std::endl;
}

void test_binomial_decrease_key() {
    std::vector<double> keys = {
        10.0,
        20.0,
        30.0,
        40.0,
        50.0
    };

    BinomialHeap heap;
    heap.build(keys);

    heap.decrease_key(4, 1.0);

    assert(heap.extract_min() == 4);

    std::cout << "Binomial decrease_key: OK" << std::endl;
}

void test_binomial_vertex_pointers() {
    std::vector<double> keys = {
        10.0,
        20.0,
        30.0,
        40.0,
        50.0,
        60.0,
        70.0,
        80.0
    };

    BinomialHeap heap;
    heap.build(keys);

    heap.decrease_key(7, 5.0);
    heap.decrease_key(6, 3.0);
    heap.decrease_key(5, 1.0);

    assert(heap.extract_min() == 5);
    assert(heap.extract_min() == 6);
    assert(heap.extract_min() == 7);

    std::cout << "Binomial vertex pointers: OK" << std::endl;
}

void test_binomial_invalid_decrease_key() {
    std::vector<double> keys = {
        1.0,
        2.0,
        3.0
    };

    BinomialHeap heap;
    heap.build(keys);

    bool exception_thrown = false;

    try {
        heap.decrease_key(1, 10.0);
    } catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    std::cout
        << "Binomial invalid decrease_key: OK"
        << std::endl;
}

void test_binomial_extracted_vertex() {
    std::vector<double> keys = {
        1.0,
        2.0,
        3.0
    };

    BinomialHeap heap;
    heap.build(keys);

    int extracted = heap.extract_min();

    assert(extracted == 0);

    bool exception_thrown = false;

    try {
        heap.decrease_key(0, 0.5);
    } catch (const std::logic_error&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    std::cout
        << "Binomial extracted vertex: OK"
        << std::endl;
}

void test_binomial_random_operations() {
    const int vertex_count = 200;
    const int decrease_operations = 1000;
    const unsigned int seed = 2026;

    std::mt19937 generator(seed);

    std::uniform_real_distribution<double> initial_distribution(
        1000.0,
        2000.0
    );

    std::uniform_real_distribution<double> decrease_distribution(
        0.0,
        900.0
    );

    std::uniform_int_distribution<int> vertex_distribution(
        0,
        vertex_count - 1
    );

    std::vector<double> keys(vertex_count);

    for (int vertex = 0; vertex < vertex_count; ++vertex) {
        keys[vertex] = initial_distribution(generator);
    }

    BinomialHeap heap;
    heap.build(keys);

    // Ejecutar muchas operaciones decrease_key aleatorias.
    for (int operation = 0;
         operation < decrease_operations;
         ++operation) {

        int vertex = vertex_distribution(generator);
        double new_key = decrease_distribution(generator);

        if (new_key < keys[vertex]) {
            keys[vertex] = new_key;
            heap.decrease_key(vertex, new_key);
        }
    }

    std::vector<bool> extracted(vertex_count, false);

    // Comparar cada extract_min con una implementacion
    // de referencia basada en busqueda lineal.
    for (int operation = 0;
         operation < vertex_count;
         ++operation) {

        int expected_vertex = -1;

        double expected_key =
            std::numeric_limits<double>::infinity();

        for (int vertex = 0;
             vertex < vertex_count;
             ++vertex) {

            if (!extracted[vertex] &&
                keys[vertex] < expected_key) {

                expected_key = keys[vertex];
                expected_vertex = vertex;
            }
        }

        int actual_vertex = heap.extract_min();

        assert(actual_vertex == expected_vertex);

        extracted[actual_vertex] = true;
    }

    assert(heap.empty());

    std::cout
        << "Binomial random operations: OK"
        << std::endl;
}

} // namespace

int main() {
    std::cout << "=== Graph tests ===" << std::endl;

    test_graph_generator();
    test_graph_generator_large();

    std::cout << std::endl;
    std::cout << "=== Binomial heap tests ===" << std::endl;

    test_binomial_extract_min();
    test_binomial_multiple_sizes();
    test_binomial_decrease_key();
    test_binomial_vertex_pointers();
    test_binomial_invalid_decrease_key();
    test_binomial_extracted_vertex();
    test_binomial_random_operations();

    std::cout << std::endl;
    std::cout
        << "Todos los tests pasaron correctamente."
        << std::endl;

    return 0;
}