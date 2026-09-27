#ifndef GRAPH_GENERATOR_HPP
#define GRAPH_GENERATOR_HPP

#include "graph.hpp"

namespace GraphGenerator {

/**
 * @brief Genera un grafo aleatorio simple, conexo, no dirigido y ponderado.
 *
 * Primero genera un arbol cobertor aleatorio con n - 1 aristas para
 * garantizar conectividad. Luego agrega aristas aleatorias hasta alcanzar
 * un total de m aristas, evitando aristas repetidas y reflexivas.
 *
 * Los pesos de las aristas se generan aleatoriamente en el intervalo (0, 1].
 *
 * @param n Cantidad de vertices del grafo.
 * @param m Cantidad de aristas del grafo.
 * @param seed Semilla utilizada para la generacion aleatoria.
 * @return Graph Grafo aleatorio generado.
 */
Graph generate_connected_graph(int n, int m, unsigned int seed);

} // namespace GraphGenerator

#endif // GRAPH_GENERATOR_HPP