#ifndef EXPERIMENT_UTILS_HPP
#define EXPERIMENT_UTILS_HPP

#include "experiment_types.hpp"
#include "graph.hpp"

#include <string>
#include <vector>

namespace ExperimentUtils {

/**
 * @brief Ejecuta ambas versiones de Prim sobre el mismo grafo.
 *
 * Se utiliza para los experimentos de costo total.
 */
std::vector<ExperimentResult> run_both(
    const Graph& graph,
    const std::string& series,
    int repetition,
    unsigned int seed
);

/**
 * @brief Ejecuta ambas versiones instrumentadas de Prim
 * y guarda sus mediciones en archivos CSV.
 *
 * Las muestras se mantienen en memoria solamente durante
 * la ejecucion correspondiente a cada heap.
 *
 * @param graph Grafo sobre el cual ejecutar Prim.
 * @param series Serie experimental.
 * @param repetition Numero de repeticion.
 * @param seed Semilla usada para generar el grafo.
 * @param binomial_filename Archivo CSV para binomial.
 * @param fibonacci_filename Archivo CSV para Fibonacci.
 */
void run_both_amortized(
    const Graph& graph,
    const std::string& series,
    int repetition,
    unsigned int seed,
    const std::string& binomial_filename,
    const std::string& fibonacci_filename
);

/**
 * @brief Guarda resultados de costo total en CSV.
 */
void write_csv(
    const std::vector<ExperimentResult>& results,
    const std::string& filename
);

} // namespace ExperimentUtils

#endif // EXPERIMENT_UTILS_HPP