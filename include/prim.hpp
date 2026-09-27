#ifndef PRIM_HPP
#define PRIM_HPP

#include "graph.hpp"

#include <vector>

/**
 * @brief Medicion acumulada despues de una llamada a decrease_key.
 *
 * El numero de llamada no se almacena explicitamente:
 * la muestra en la posicion i corresponde a la llamada i + 1.
 */
struct DecreaseKeySample {
    /**
     * @brief Tiempo acumulado en decrease_key, en nanosegundos.
     */
    long long cumulative_time_ns;

    /**
     * @brief Operaciones estructurales acumuladas.
     *
     * Binomial: intercambios.
     * Fibonacci: cortes en cascada.
     */
    long long structural_operations;
};

/**
 * @brief Resultado de una ejecucion del algoritmo de Prim.
 */
struct PrimResult {
    std::vector<int> parent;
    std::vector<double> cost;

    double total_weight;

    long long decrease_key_calls;
    long long decrease_key_time_ns;

    long long structural_operations;

    std::vector<DecreaseKeySample> decrease_key_samples;
};

/**
 * @brief Ejecuta Prim usando una cola binomial.
 *
 * @param graph Grafo conexo no dirigido.
 * @param root Vertice raiz.
 * @param collect_samples Indica si se almacenan muestras
 *        para el experimento amortizado.
 */
PrimResult prim_binomial(
    const Graph& graph,
    int root,
    bool collect_samples = false
);

/**
 * @brief Ejecuta Prim usando una cola de Fibonacci.
 *
 * @param graph Grafo conexo no dirigido.
 * @param root Vertice raiz.
 * @param collect_samples Indica si se almacenan muestras
 *        para el experimento amortizado.
 */
PrimResult prim_fibonacci(
    const Graph& graph,
    int root,
    bool collect_samples = false
);

#endif // PRIM_HPP