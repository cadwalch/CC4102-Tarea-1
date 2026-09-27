#include "prim.hpp"

#include "binomial_heap.hpp"
#include "fibonacci_heap.hpp"

#include <chrono>
#include <limits>
#include <stdexcept>
#include <vector>

PrimResult prim_binomial(
    const Graph& graph,
    int root,
    bool collect_samples
) {
    int n = graph.vertex_count();

    if (n == 0) {
        throw std::invalid_argument(
            "Prim requiere un grafo no vacio"
        );
    }

    if (root < 0 || root >= n) {
        throw std::out_of_range(
            "Raiz fuera de rango"
        );
    }

    const double infinity =
        std::numeric_limits<double>::infinity();

    std::vector<double> cost(
        n,
        infinity
    );

    std::vector<int> parent(
        n,
        -1
    );

    cost[root] = 0.0;

    BinomialHeap heap;
    heap.build(cost);

    double total_weight = 0.0;

    long long decrease_key_calls = 0;
    long long decrease_key_time_ns = 0;

    std::vector<DecreaseKeySample> samples;

    while (!heap.empty()) {
        int vertex =
            heap.extract_min();

        total_weight +=
            cost[vertex];

        for (const Edge& edge :
             graph.neighbors(vertex)) {

            int neighbor =
                edge.to;

            if (heap.contains(neighbor) &&
                edge.weight < cost[neighbor]) {

                cost[neighbor] =
                    edge.weight;

                parent[neighbor] =
                    vertex;

                if (collect_samples) {
                    auto start =
                        std::chrono::steady_clock::now();

                    heap.decrease_key(
                        neighbor,
                        edge.weight
                    );

                    auto end =
                        std::chrono::steady_clock::now();

                    decrease_key_time_ns +=
                        std::chrono::duration_cast<
                            std::chrono::nanoseconds
                        >(
                            end - start
                        ).count();

                    decrease_key_calls++;

                    samples.push_back({
                        decrease_key_time_ns,
                        heap.swap_count()
                    });

                } else {
                    heap.decrease_key(
                        neighbor,
                        edge.weight
                    );

                    decrease_key_calls++;
                }
            }
        }
    }

    return {
        parent,
        cost,
        total_weight,
        decrease_key_calls,
        decrease_key_time_ns,
        heap.swap_count(),
        samples
    };
}

PrimResult prim_fibonacci(
    const Graph& graph,
    int root,
    bool collect_samples
) {
    int n = graph.vertex_count();

    if (n == 0) {
        throw std::invalid_argument(
            "Prim requiere un grafo no vacio"
        );
    }

    if (root < 0 || root >= n) {
        throw std::out_of_range(
            "Raiz fuera de rango"
        );
    }

    const double infinity =
        std::numeric_limits<double>::infinity();

    std::vector<double> cost(
        n,
        infinity
    );

    std::vector<int> parent(
        n,
        -1
    );

    cost[root] = 0.0;

    FibonacciHeap heap;
    heap.build(cost);

    double total_weight = 0.0;

    long long decrease_key_calls = 0;
    long long decrease_key_time_ns = 0;

    std::vector<DecreaseKeySample> samples;

    while (!heap.empty()) {
        int vertex =
            heap.extract_min();

        total_weight +=
            cost[vertex];

        for (const Edge& edge :
             graph.neighbors(vertex)) {

            int neighbor =
                edge.to;

            if (heap.contains(neighbor) &&
                edge.weight < cost[neighbor]) {

                cost[neighbor] =
                    edge.weight;

                parent[neighbor] =
                    vertex;

                if (collect_samples) {
                    auto start =
                        std::chrono::steady_clock::now();

                    heap.decrease_key(
                        neighbor,
                        edge.weight
                    );

                    auto end =
                        std::chrono::steady_clock::now();

                    decrease_key_time_ns +=
                        std::chrono::duration_cast<
                            std::chrono::nanoseconds
                        >(
                            end - start
                        ).count();

                    decrease_key_calls++;

                    samples.push_back({
                        decrease_key_time_ns,
                        heap.cascading_cut_count()
                    });


                } else {
                    heap.decrease_key(
                        neighbor,
                        edge.weight
                    );

                    decrease_key_calls++;
                }
            }
        }
    }

    return {
        parent,
        cost,
        total_weight,
        decrease_key_calls,
        decrease_key_time_ns,
        heap.cascading_cut_count(),
        samples
    };
}