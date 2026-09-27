#ifndef PRIM_HPP
#define PRIM_HPP

#include "graph.hpp"

#include <vector>

/**
 * @brief Resultado de ejecutar el algoritmo de Prim.
 */
struct PrimResult {
    /**
     * parent[v] corresponde al padre del vertice v en el MST.
     * La raiz tiene padre -1.
     */
    std::vector<int> parent;

    /**
     * cost[v] corresponde al peso de la arista que conecta v
     * con su padre en el MST.
     */
    std::vector<double> cost;

    /**
     * Suma de los pesos de las aristas del MST.
     */
    double total_weight;
};

/**
 * @brief Ejecuta el algoritmo de Prim utilizando una cola binomial.
 *
 * @param graph Grafo conexo, no dirigido y ponderado.
 * @param root Vertice utilizado como raiz del MST.
 * @return Resultado del algoritmo de Prim.
 */
PrimResult prim_binomial(const Graph& graph, int root);

#endif // PRIM_HPP