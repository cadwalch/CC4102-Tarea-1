#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

/**
 * @brief Arista de un grafo ponderado.
 *
 * Representa una arista desde un vertice hacia otro vertice del grafo.
 * Como el grafo es no dirigido, cada arista se almacena en ambas listas
 * de adyacencia.
 */
struct Edge {
    int to;
    double weight;
};

/**
 * @brief Grafo no dirigido y ponderado representado mediante listas de adyacencia.
 *
 * Los vertices se identifican mediante enteros en el rango [0, n - 1].
 * Cada posicion de adjacency contiene las aristas incidentes al vertice
 * correspondiente.
 */
class Graph {
public:
    /**
     * @brief Construye un grafo sin aristas.
     *
     * @param vertex_count Cantidad de vertices del grafo.
     */
    explicit Graph(int vertex_count);

    /**
     * @brief Agrega una arista no dirigida al grafo.
     *
     * La arista se almacena tanto en la lista de adyacencia de u como
     * en la de v.
     *
     * @param u Primer vertice de la arista.
     * @param v Segundo vertice de la arista.
     * @param weight Peso asociado a la arista.
     */
    void add_edge(int u, int v, double weight);

    /**
     * @brief Obtiene las aristas incidentes a un vertice.
     *
     * @param vertex Vertice cuya lista de adyacencia se quiere consultar.
     * @return Referencia constante a la lista de aristas del vertice.
     */
    const std::vector<Edge>& neighbors(int vertex) const;

    /**
     * @brief Obtiene la cantidad de vertices del grafo.
     *
     * @return Cantidad de vertices.
     */
    int vertex_count() const;

    /**
     * @brief Obtiene la cantidad de aristas del grafo.
     *
     * Cada arista no dirigida se cuenta una sola vez, aunque se almacene
     * en dos listas de adyacencia.
     *
     * @return Cantidad de aristas.
     */
    int edge_count() const;

private:
    int vertex_count_;
    int edge_count_;
    std::vector<std::vector<Edge>> adjacency_;
};

#endif // GRAPH_HPP