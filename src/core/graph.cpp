#include "graph.hpp"

#include <stdexcept>

Graph::Graph(int vertex_count)
    : vertex_count_(vertex_count),
      edge_count_(0),
      adjacency_(vertex_count) {
    if (vertex_count < 0) {
        throw std::invalid_argument(
            "La cantidad de vertices no puede ser negativa"
        );
    }
}

void Graph::add_edge(int u, int v, double weight) {
    if (u < 0 || u >= vertex_count_ ||
        v < 0 || v >= vertex_count_) {
        throw std::out_of_range("Vertice fuera de rango");
    }

    if (u == v) {
        throw std::invalid_argument(
            "No se permiten aristas reflexivas"
        );
    }

    if (weight <= 0.0) {
        throw std::invalid_argument(
            "El peso de una arista debe ser mayor que 0"
        );
    }

    adjacency_[u].push_back({v, weight});
    adjacency_[v].push_back({u, weight});

    edge_count_++;
}

const std::vector<Edge>& Graph::neighbors(int vertex) const {
    if (vertex < 0 || vertex >= vertex_count_) {
        throw std::out_of_range("Vertice fuera de rango");
    }

    return adjacency_[vertex];
}

int Graph::vertex_count() const {
    return vertex_count_;
}

int Graph::edge_count() const {
    return edge_count_;
}