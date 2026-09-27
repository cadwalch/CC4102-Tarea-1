#include "graph_generator.hpp"

#include <random>
#include <stdexcept>
#include <unordered_set>

namespace {

long long edge_id(int u, int v, int n) {
    if (u > v) {
        int temp = u;
        u = v;
        v = temp;
    }

    return static_cast<long long>(u) * n + v;
}

double generate_weight(std::mt19937& generator) {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double weight = 0.0;

    while (weight == 0.0) {
        weight = distribution(generator);
    }

    return weight;
}

} // namespace

namespace GraphGenerator {

Graph generate_connected_graph(int n, int m, unsigned int seed) {
    if (n <= 0) {
        throw std::invalid_argument(
            "La cantidad de vertices debe ser mayor que 0"
        );
    }

    long long max_edges =
        static_cast<long long>(n) * (n - 1) / 2;

    if (m < n - 1) {
        throw std::invalid_argument(
            "Un grafo conexo necesita al menos n - 1 aristas"
        );
    }

    if (m > max_edges) {
        throw std::invalid_argument(
            "La cantidad de aristas excede el maximo de un grafo simple"
        );
    }

    Graph graph(n);

    std::mt19937 generator(seed);

    std::unordered_set<long long> existing_edges;
    existing_edges.reserve(static_cast<std::size_t>(m) * 2);

    // Construir primero un arbol cobertor para garantizar conectividad.
    for (int vertex = 1; vertex < n; ++vertex) {
        std::uniform_int_distribution<int> parent_distribution(
            0, vertex - 1
        );

        int parent = parent_distribution(generator);
        double weight = generate_weight(generator);

        graph.add_edge(vertex, parent, weight);
        existing_edges.insert(edge_id(vertex, parent, n));
    }

    // Agregar las aristas restantes evitando repeticiones y loops.
    std::uniform_int_distribution<int> vertex_distribution(0, n - 1);

    while (graph.edge_count() < m) {
        int u = vertex_distribution(generator);
        int v = vertex_distribution(generator);

        if (u == v) {
            continue;
        }

        long long id = edge_id(u, v, n);

        if (existing_edges.find(id) != existing_edges.end()) {
            continue;
        }

        double weight = generate_weight(generator);

        graph.add_edge(u, v, weight);
        existing_edges.insert(id);
    }

    return graph;
}

} // namespace GraphGenerator