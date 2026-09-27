#include "prim.hpp"

#include "binomial_heap.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

PrimResult prim_binomial(const Graph& graph, int root) {
    int n = graph.vertex_count();

    if (n == 0) {
        throw std::invalid_argument(
            "No se puede ejecutar Prim sobre un grafo vacio"
        );
    }

    if (root < 0 || root >= n) {
        throw std::out_of_range(
            "La raiz de Prim esta fuera de rango"
        );
    }

    const double infinity =
        std::numeric_limits<double>::infinity();

    std::vector<double> cost(n, infinity);
    std::vector<int> parent(n, -1);

    cost[root] = 0.0;

    BinomialHeap heap;
    heap.build(cost);

    double total_weight = 0.0;

    while (!heap.empty()) {
        int vertex = heap.extract_min();

        total_weight += cost[vertex];

        for (const Edge& edge : graph.neighbors(vertex)) {
            int neighbor = edge.to;

            if (heap.contains(neighbor) &&
                edge.weight < cost[neighbor]) {

                cost[neighbor] = edge.weight;
                parent[neighbor] = vertex;

                heap.decrease_key(
                    neighbor,
                    edge.weight
                );
            }
        }
    }

    return {
        parent,
        cost,
        total_weight
    };
}