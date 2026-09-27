#include "binomial_heap.hpp"
#include "fibonacci_heap.hpp"
#include "graph.hpp"

#include <iostream>
#include <vector>

int main() {
    std::cout
        << "sizeof(int) = "
        << sizeof(int)
        << " bytes\n";

    std::cout
        << "sizeof(double) = "
        << sizeof(double)
        << " bytes\n";

    std::cout
        << "sizeof(void*) = "
        << sizeof(void*)
        << " bytes\n";

    std::cout
        << "sizeof(Edge) = "
        << sizeof(Edge)
        << " bytes\n";

    std::cout
        << "sizeof(vector<Edge>) = "
        << sizeof(std::vector<Edge>)
        << " bytes\n";

    std::cout
        << "sizeof(BinomialNode) = "
        << sizeof(BinomialNode)
        << " bytes\n";

    std::cout
        << "sizeof(FibonacciNode) = "
        << sizeof(FibonacciNode)
        << " bytes\n";

    return 0;
}