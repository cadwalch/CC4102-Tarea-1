#ifndef EXPERIMENT_TYPES_HPP
#define EXPERIMENT_TYPES_HPP

#include <string>

/**
 * @brief Resultado de una ejecucion experimental de Prim.
 */
struct ExperimentResult {
    std::string heap_type;
    std::string series;

    int vertex_count;
    int edge_count;
    int repetition;

    unsigned int seed;

    double mst_weight;

    long long total_time_ns;

    long long decrease_key_calls;
    long long decrease_key_time_ns;

    /**
     * Binomial: intercambios.
     * Fibonacci: cortes en cascada.
     */
    long long structural_operations;
};

#endif // EXPERIMENT_TYPES_HPP