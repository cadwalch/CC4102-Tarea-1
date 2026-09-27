#include "graph.hpp"
#include "graph_generator.hpp"

#include <cassert>
#include <iostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

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

int main() {
    const int n = 10000;
    const int m = 50000;
    const unsigned int seed = 42;

    Graph graph =
        GraphGenerator::generate_connected_graph(n, m, seed);

    assert(graph.vertex_count() == n);
    assert(graph.edge_count() == m);
    assert(is_connected(graph));
    assert(is_simple(graph));

    std::cout << "Vertices: " << graph.vertex_count() << std::endl;
    std::cout << "Aristas: " << graph.edge_count() << std::endl;
    std::cout << "Conexo: si" << std::endl;
    std::cout << "Simple: si" << std::endl;
    std::cout << "Todos los tests pasaron correctamente." << std::endl;

    return 0;
}