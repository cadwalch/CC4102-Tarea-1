#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP

#include <vector>

/**
 * @brief Nodo de una cola de Fibonacci.
 *
 * Cada nodo almacena un par (key, vertex), informacion estructural
 * del arbol al que pertenece y su marca para los cortes en cascada.
 *
 * Los nodos de una misma lista se conectan mediante una lista
 * doblemente enlazada circular.
 */
struct FibonacciNode {
    double key;
    int vertex;
    int degree;
    bool mark;

    FibonacciNode* parent;
    FibonacciNode* child;
    FibonacciNode* left;
    FibonacciNode* right;
};

/**
 * @brief Cola de prioridad minima implementada mediante un heap de Fibonacci.
 *
 * Mantiene acceso directo al nodo asociado a cada vertice para poder
 * realizar decrease_key sin buscar el vertice dentro de la estructura.
 */
class FibonacciHeap {
public:
    /**
     * @brief Construye una cola de Fibonacci vacia.
     */
    FibonacciHeap();

    /**
     * @brief Libera todos los nodos almacenados en la cola.
     */
    ~FibonacciHeap();

    /**
     * @brief Construye la cola a partir de un arreglo de claves iniciales.
     *
     * El indice de cada clave corresponde al vertice asociado.
     *
     * @param keys Claves iniciales de los vertices.
     */
    void build(const std::vector<double>& keys);

    /**
     * @brief Extrae y elimina el elemento con menor clave.
     *
     * @return Vertice asociado al elemento minimo.
     */
    int extract_min();

    /**
     * @brief Disminuye la clave asociada a un vertice.
     *
     * Si se viola el orden de heap, el nodo se corta de su padre
     * y se realizan los cortes en cascada correspondientes.
     *
     * @param vertex Vertice cuya clave se quiere disminuir.
     * @param new_key Nueva clave del vertice.
     */
    void decrease_key(int vertex, double new_key);

    /**
     * @brief Indica si un vertice se encuentra actualmente en la cola.
     *
     * @param vertex Vertice que se quiere consultar.
     * @return true si el vertice esta en la cola, false en caso contrario.
     */
    bool contains(int vertex) const;

    /**
     * @brief Indica si la cola esta vacia.
     *
     * @return true si la cola no contiene elementos, false en caso contrario.
     */
    bool empty() const;

    /**
     * @brief Retorna la cantidad total de cortes realizados.
     *
     * @return Cantidad acumulada de cortes.
     */
    long long cut_count() const;

    /**
     * @brief Retorna la cantidad de cortes realizados como parte
     * de una cascada.
     *
     * @return Cantidad acumulada de cortes en cascada.
     */
    long long cascading_cut_count() const;


    FibonacciHeap(const FibonacciHeap&) = delete;

    FibonacciHeap& operator=(
        const FibonacciHeap&
    ) = delete;

private:
    FibonacciNode* min_;
    int size_;

    /**
     * @brief Acceso directo desde un vertice a su nodo actual en la cola.
     */
    std::vector<FibonacciNode*> node_by_vertex_;

    /**
     * @brief Inserta un nodo en la lista de raices.
     *
     * @param node Nodo que se quiere agregar.
     */
    void add_to_root_list(FibonacciNode* node);

    /**
     * @brief Une dos arboles de Fibonacci.
     *
     * El nodo child deja de ser raiz y pasa a ser hijo de parent.
     *
     * @param child Raiz que se convertira en hijo.
     * @param parent Raiz que se mantendra como padre.
     */
    void link(FibonacciNode* child, FibonacciNode* parent);

    /**
     * @brief Consolida las raices para que no existan dos
     * arboles con el mismo grado.
     */
    void consolidate();

    /**
     * @brief Corta un nodo de su padre y lo mueve a la lista de raices.
     *
     * @param node Nodo que se quiere cortar.
     * @param parent Padre actual del nodo.
     */
    void cut(FibonacciNode* node, FibonacciNode* parent);

    /**
     * @brief Realiza los cortes en cascada a partir de un nodo.
     *
     * @param node Nodo desde el cual comienza el proceso.
     */
    void cascading_cut(FibonacciNode* node);

    /**
     * @brief Libera recursivamente los nodos de una lista circular.
     *
     * @param node Nodo perteneciente a la lista que se quiere liberar.
     */
    void delete_list(FibonacciNode* node);

    long long cut_count_;
    long long cascading_cut_count_;
};

#endif // FIBONACCI_HEAP_HPP