#include "experiment_types.hpp"
#include "experiment_utils.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct ExperimentConfig {
    std::string series;
    int i;
    int j;
};

int power_of_two(int exponent) {
    return 1 << exponent;
}

unsigned int make_seed(
    int i,
    int j,
    int repetition
) {
    return static_cast<unsigned int>(
        i * 1000000 +
        j * 1000 +
        repetition
    );
}

void run_total_series(
    const std::vector<ExperimentConfig>& configs,
    int repetitions,
    std::vector<ExperimentResult>& results
) {
    for (const ExperimentConfig& config : configs) {
        int vertex_count =
            power_of_two(config.i);

        int edge_count =
            power_of_two(config.j);

        std::cout
            << "Serie " << config.series
            << ": V=2^" << config.i
            << "=" << vertex_count
            << ", E=2^" << config.j
            << "=" << edge_count
            << std::endl;

        for (int repetition = 0;
             repetition < repetitions;
             ++repetition) {

            unsigned int seed =
                make_seed(
                    config.i,
                    config.j,
                    repetition
                );

            std::cout
                << "  Repeticion "
                << repetition + 1
                << "/"
                << repetitions
                << std::endl;

            Graph graph =
                GraphGenerator::generate_connected_graph(
                    vertex_count,
                    edge_count,
                    seed
                );

            std::vector<ExperimentResult>
                current_results =
                    ExperimentUtils::run_both(
                        graph,
                        config.series,
                        repetition,
                        seed
                    );

            results.insert(
                results.end(),
                current_results.begin(),
                current_results.end()
            );
        }
    }
}

void run_amortized_series(
    const std::vector<ExperimentConfig>& configs,
    int repetitions,
    const std::string& output_directory
) {
    for (const ExperimentConfig& config : configs) {
        int vertex_count =
            power_of_two(config.i);

        int edge_count =
            power_of_two(config.j);

        std::cout
            << "Serie " << config.series
            << ": V=2^" << config.i
            << "=" << vertex_count
            << ", E=2^" << config.j
            << "=" << edge_count
            << std::endl;

        for (int repetition = 0;
             repetition < repetitions;
             ++repetition) {

            unsigned int seed =
                make_seed(
                    config.i,
                    config.j,
                    repetition
                );

            std::cout
                << "  Repeticion "
                << repetition + 1
                << "/"
                << repetitions
                << std::endl;

            Graph graph =
                GraphGenerator::generate_connected_graph(
                    vertex_count,
                    edge_count,
                    seed
                );

            std::string base_filename =
                output_directory +
                "/amortized_" +
                config.series +
                "_i" +
                std::to_string(config.i) +
                "_j" +
                std::to_string(config.j) +
                "_r" +
                std::to_string(repetition);

            std::string binomial_filename =
                base_filename +
                "_binomial.csv";

            std::string fibonacci_filename =
                base_filename +
                "_fibonacci.csv";

            ExperimentUtils::run_both_amortized(
                graph,
                config.series,
                repetition,
                seed,
                binomial_filename,
                fibonacci_filename
            );
        }
    }
}

void run_small_experiments() {
    std::cout
        << "=== Experimentos pequenos ==="
        << std::endl;

    std::vector<ExperimentConfig> configs = {
        {"TEST", 8, 9},
        {"TEST", 9, 10}
    };

    std::vector<ExperimentResult> results;

    run_total_series(
        configs,
        2,
        results
    );

    ExperimentUtils::write_csv(
        results,
        "results/raw/test_results.csv"
    );

    std::cout
        << "Resultados guardados en "
        << "results/raw/test_results.csv"
        << std::endl;
}

void run_small_amortized_experiment() {
    std::cout
        << "=== Prueba amortizada pequena ==="
        << std::endl;

    std::vector<ExperimentConfig> configs = {
        {"TEST", 8, 10}
    };

    run_amortized_series(
        configs,
        1,
        "results/raw"
    );

    std::cout
        << "Prueba amortizada terminada."
        << std::endl;
}

void run_total_cost_experiments() {
    std::cout
        << "=== Experimentos de costo total ==="
        << std::endl;

    /*
     * Serie A:
     * i = 20 fijo
     * j = 20, 21, 22, 23, 24
     *
     * Serie B:
     * j = 24 fijo
     * i = 18, 19, 20, 21, 22
     */
    std::vector<ExperimentConfig> configs = {
        {"A", 20, 20},
        {"A", 20, 21},
        {"A", 20, 22},
        {"A", 20, 23},
        {"A", 20, 24},

        {"B", 18, 24},
        {"B", 19, 24},
        {"B", 20, 24},
        {"B", 21, 24},
        {"B", 22, 24}
    };

    std::vector<ExperimentResult> results;

    run_total_series(
        configs,
        10,
        results
    );

    ExperimentUtils::write_csv(
        results,
        "results/raw/total_cost.csv"
    );

    std::cout
        << "Resultados guardados en "
        << "results/raw/total_cost.csv"
        << std::endl;
}

void run_amortized_experiments() {
    std::cout
        << "=== Experimentos de costo amortizado ==="
        << std::endl;

    /*
     * Serie C:
     * i = 18 fijo
     * j = 18, 19, 20, 21, 22
     *
     * Serie D:
     * j = 22 fijo
     * i = 14, 15, 16, 17, 18
     */
    std::vector<ExperimentConfig> configs = {
        {"C", 18, 18},
        {"C", 18, 19},
        {"C", 18, 20},
        {"C", 18, 21},
        {"C", 18, 22},

        {"D", 14, 22},
        {"D", 15, 22},
        {"D", 16, 22},
        {"D", 17, 22},
        {"D", 18, 22}
    };

    run_amortized_series(
        configs,
        10,
        "results/raw"
    );

    std::cout
        << "Experimentos amortizados terminados."
        << std::endl;
}

void print_usage(const char* program_name) {
    std::cout
        << "Uso:\n"
        << "  " << program_name
        << " --small\n"
        << "  " << program_name
        << " --small-amortized\n"
        << "  " << program_name
        << " --total\n"
        << "  " << program_name
        << " --amortized\n"
        << "  " << program_name
        << " --all\n";
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        std::filesystem::create_directories(
            "results/raw"
        );

        if (argc != 2) {
            print_usage(argv[0]);
            return 1;
        }

        std::string mode = argv[1];

        if (mode == "--small") {
            run_small_experiments();

        } else if (mode == "--small-amortized") {
            run_small_amortized_experiment();

        } else if (mode == "--total") {
            run_total_cost_experiments();

        } else if (mode == "--amortized") {
            run_amortized_experiments();

        } else if (mode == "--all") {
            run_total_cost_experiments();
            run_amortized_experiments();

        } else {
            print_usage(argv[0]);
            return 1;
        }

    } catch (const std::exception& exception) {
        std::cerr
            << "Error: "
            << exception.what()
            << std::endl;

        return 1;
    }

    return 0;
}