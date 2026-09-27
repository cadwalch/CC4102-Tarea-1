#include "graph.hpp"

#include <iostream>

int main() {
    Graph graph(4);

    graph.add_edge(0, 1, 0.5);
    graph.add_edge(0, 2, 0.3);
    graph.add_edge(1, 3, 0.8);

    std::cout << "Vertices: " << graph.vertex_count() << std::endl;
    std::cout << "Aristas: " << graph.edge_count() << std::endl;

    for (int v = 0; v < graph.vertex_count(); ++v) {
        std::cout << v << ":";

        for (const Edge& edge : graph.neighbors(v)) {
            std::cout << " (" << edge.to
                      << ", " << edge.weight << ")";
        }

        std::cout << std::endl;
    }

    return 0;
}