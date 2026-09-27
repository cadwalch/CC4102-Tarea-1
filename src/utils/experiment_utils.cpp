#include "experiment_utils.hpp"

#include "prim.hpp"

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <vector>

namespace {

/**
 * @brief Escribe las muestras acumuladas de decrease_key
 * en un archivo CSV.
 */
void write_amortized_samples(
    const PrimResult& result,
    const std::string& heap_type,
    const std::string& series,
    int vertex_count,
    int edge_count,
    int repetition,
    unsigned int seed,
    const std::string& filename
) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir el archivo CSV amortizado: "
            + filename
        );
    }

    file
        << "heap_type,"
        << "series,"
        << "vertex_count,"
        << "edge_count,"
        << "repetition,"
        << "seed,"
        << "call_count,"
        << "cumulative_time_ns,"
        << "structural_operations\n";

    /*
     * call_count no se almacena dentro de cada muestra.
     * La muestra i corresponde a la llamada i + 1.
     */
    for (std::size_t i = 0;
         i < result.decrease_key_samples.size();
         ++i) {

        const DecreaseKeySample& sample =
            result.decrease_key_samples[i];

        long long call_count =
            static_cast<long long>(i) + 1;

        file
            << heap_type << ","
            << series << ","
            << vertex_count << ","
            << edge_count << ","
            << repetition << ","
            << seed << ","
            << call_count << ","
            << sample.cumulative_time_ns << ","
            << sample.structural_operations
            << "\n";
    }
}

} // namespace


namespace ExperimentUtils {

std::vector<ExperimentResult> run_both(
    const Graph& graph,
    const std::string& series,
    int repetition,
    unsigned int seed
) {
    std::vector<ExperimentResult> results;

    // --------------------------------------------------------
    // Prim con Binomial Heap
    // --------------------------------------------------------

    auto binomial_start =
        std::chrono::steady_clock::now();

    PrimResult binomial =
        prim_binomial(
            graph,
            0
        );

    auto binomial_end =
        std::chrono::steady_clock::now();

    long long binomial_time_ns =
        std::chrono::duration_cast<
            std::chrono::nanoseconds
        >(
            binomial_end -
            binomial_start
        ).count();

    // --------------------------------------------------------
    // Prim con Fibonacci Heap
    // --------------------------------------------------------

    auto fibonacci_start =
        std::chrono::steady_clock::now();

    PrimResult fibonacci =
        prim_fibonacci(
            graph,
            0
        );

    auto fibonacci_end =
        std::chrono::steady_clock::now();

    long long fibonacci_time_ns =
        std::chrono::duration_cast<
            std::chrono::nanoseconds
        >(
            fibonacci_end -
            fibonacci_start
        ).count();

    // --------------------------------------------------------
    // Verificacion del MST
    // --------------------------------------------------------

    const double epsilon = 1e-9;

    if (std::abs(
            binomial.total_weight -
            fibonacci.total_weight
        ) > epsilon) {

        throw std::logic_error(
            "Binomial y Fibonacci produjeron "
            "pesos de MST distintos"
        );
    }

    // --------------------------------------------------------
    // Guardar resultados
    // --------------------------------------------------------

    results.push_back({
        "binomial",
        series,
        graph.vertex_count(),
        graph.edge_count(),
        repetition,
        seed,
        binomial.total_weight,
        binomial_time_ns,
        binomial.decrease_key_calls,
        binomial.decrease_key_time_ns,
        binomial.structural_operations
    });

    results.push_back({
        "fibonacci",
        series,
        graph.vertex_count(),
        graph.edge_count(),
        repetition,
        seed,
        fibonacci.total_weight,
        fibonacci_time_ns,
        fibonacci.decrease_key_calls,
        fibonacci.decrease_key_time_ns,
        fibonacci.structural_operations
    });

    return results;
}


void run_both_amortized(
    const Graph& graph,
    const std::string& series,
    int repetition,
    unsigned int seed,
    const std::string& binomial_filename,
    const std::string& fibonacci_filename
) {
    double binomial_mst_weight = 0.0;

    // --------------------------------------------------------
    // Binomial
    // --------------------------------------------------------

    /*
     * El scope permite liberar las muestras de Binomial
     * antes de ejecutar Fibonacci.
     */
    {
        PrimResult binomial =
            prim_binomial(
                graph,
                0,
                true
            );

        binomial_mst_weight =
            binomial.total_weight;

        write_amortized_samples(
            binomial,
            "binomial",
            series,
            graph.vertex_count(),
            graph.edge_count(),
            repetition,
            seed,
            binomial_filename
        );
    }

    double fibonacci_mst_weight = 0.0;

    // --------------------------------------------------------
    // Fibonacci
    // --------------------------------------------------------

    {
        PrimResult fibonacci =
            prim_fibonacci(
                graph,
                0,
                true
            );

        fibonacci_mst_weight =
            fibonacci.total_weight;

        write_amortized_samples(
            fibonacci,
            "fibonacci",
            series,
            graph.vertex_count(),
            graph.edge_count(),
            repetition,
            seed,
            fibonacci_filename
        );
    }

    // --------------------------------------------------------
    // Verificacion del MST
    // --------------------------------------------------------

    const double epsilon = 1e-9;

    if (std::abs(
            binomial_mst_weight -
            fibonacci_mst_weight
        ) > epsilon) {

        throw std::logic_error(
            "Binomial y Fibonacci produjeron "
            "pesos de MST distintos"
        );
    }
}


void write_csv(
    const std::vector<ExperimentResult>& results,
    const std::string& filename
) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir el archivo CSV: "
            + filename
        );
    }

    file
        << "heap_type,"
        << "series,"
        << "vertex_count,"
        << "edge_count,"
        << "repetition,"
        << "seed,"
        << "mst_weight,"
        << "total_time_ns,"
        << "decrease_key_calls,"
        << "decrease_key_time_ns,"
        << "structural_operations\n";

    file << std::setprecision(17);

    for (const ExperimentResult& result :
         results) {

        file
            << result.heap_type << ","
            << result.series << ","
            << result.vertex_count << ","
            << result.edge_count << ","
            << result.repetition << ","
            << result.seed << ","
            << result.mst_weight << ","
            << result.total_time_ns << ","
            << result.decrease_key_calls << ","
            << result.decrease_key_time_ns << ","
            << result.structural_operations
            << "\n";
    }
}

} // namespace ExperimentUtils