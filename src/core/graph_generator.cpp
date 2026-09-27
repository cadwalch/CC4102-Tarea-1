#include "graph_generator.hpp"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace {

/**
 * @brief Indica si ya existe la arista no dirigida (u, v).
 *
 * Se revisa la lista de adyacencia del vertice de menor grado
 * para reducir el numero esperado de comparaciones.
 */
bool edge_exists(
    const Graph& graph,
    int u,
    int v
) {
    const auto& neighbors_u =
        graph.neighbors(u);

    const auto& neighbors_v =
        graph.neighbors(v);

    if (neighbors_u.size() <= neighbors_v.size()) {
        for (const Edge& edge : neighbors_u) {
            if (edge.to == v) {
                return true;
            }
        }
    } else {
        for (const Edge& edge : neighbors_v) {
            if (edge.to == u) {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Genera un peso aleatorio en (0, 1].
 */
double random_weight(
    std::mt19937& generator,
    std::uniform_real_distribution<double>& distribution
) {
    double weight = 0.0;

    while (weight <= 0.0) {
        weight = distribution(generator);
    }

    return weight;
}

} // namespace

namespace GraphGenerator {

Graph generate_connected_graph(
    int vertex_count,
    int edge_count,
    unsigned int seed
) {
    if (vertex_count <= 0) {
        throw std::invalid_argument(
            "El numero de vertices debe ser positivo"
        );
    }

    long long maximum_edges =
        static_cast<long long>(vertex_count) *
        static_cast<long long>(vertex_count - 1) /
        2;

    if (edge_count < vertex_count - 1) {
        throw std::invalid_argument(
            "No hay suficientes aristas para "
            "generar un grafo conexo"
        );
    }

    if (static_cast<long long>(edge_count) >
        maximum_edges) {

        throw std::invalid_argument(
            "Demasiadas aristas para un grafo simple"
        );
    }

    Graph graph(vertex_count);

    std::mt19937 generator(seed);

    std::uniform_real_distribution<double>
        weight_distribution(0.0, 1.0);

    /*
     * Primero construimos un arbol generador.
     *
     * Para cada vertice v > 0 elegimos un padre
     * aleatorio entre [0, v - 1].
     *
     * Esto garantiza conectividad y no puede producir
     * aristas duplicadas dentro del arbol.
     */
    for (int vertex = 1;
         vertex < vertex_count;
         ++vertex) {

        std::uniform_int_distribution<int>
            parent_distribution(
                0,
                vertex - 1
            );

        int parent =
            parent_distribution(generator);

        graph.add_edge(
            vertex,
            parent,
            random_weight(
                generator,
                weight_distribution
            )
        );
    }

    /*
     * Luego agregamos las aristas restantes.
     */
    std::uniform_int_distribution<int>
        vertex_distribution(
            0,
            vertex_count - 1
        );

    while (graph.edge_count() < edge_count) {
        int u =
            vertex_distribution(generator);

        int v =
            vertex_distribution(generator);

        if (u == v) {
            continue;
        }

        if (edge_exists(graph, u, v)) {
            continue;
        }

        graph.add_edge(
            u,
            v,
            random_weight(
                generator,
                weight_distribution
            )
        );
    }

    return graph;
}

} // namespace GraphGenerator