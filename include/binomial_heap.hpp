#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <vector>

/**
 * @brief Nodo de una cola binomial.
 *
 * Cada nodo almacena un par (key, vertex) y los punteros necesarios
 * para representar un arbol binomial mediante la relacion
 * padre-hijo-hermano.
 */
struct BinomialNode {
    double key;
    int vertex;
    int degree;

    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;
};

/**
 * @brief Cola de prioridad minima implementada mediante arboles binomiales.
 *
 * La cola almacena pares (key, vertex) ordenados por key y mantiene
 * acceso directo al nodo asociado a cada vertice para implementar
 * decrease_key eficientemente.
 */
class BinomialHeap {
public:
    /**
     * @brief Construye una cola binomial vacia.
     */
    BinomialHeap();

    /**
     * @brief Libera todos los nodos almacenados en la cola.
     */
    ~BinomialHeap();

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
     * Si el nuevo valor rompe el orden de heap, el contenido del nodo
     * se intercambia con el de sus ancestros hasta restaurar el orden.
     *
     * @param vertex Vertice cuya clave se quiere disminuir.
     * @param new_key Nueva clave del vertice.
     */
    void decrease_key(int vertex, double new_key);

    /**
     * @brief Indica si la cola esta vacia.
     *
     * @return true si la cola no contiene elementos, false en caso contrario.
     */
    bool empty() const;

private:
    BinomialNode* head_;
    BinomialNode* min_;
    int size_;

    /**
     * @brief Acceso directo desde un vertice a su nodo actual en la cola.
     */
    std::vector<BinomialNode*> node_by_vertex_;

    /**
     * @brief Une dos arboles binomiales del mismo grado.
     *
     * El nodo con mayor clave se convierte en hijo del nodo con
     * menor clave.
     *
     * @param child Raiz que se convertira en hijo.
     * @param parent Raiz que se mantendra como padre.
     */
    void link(BinomialNode* child, BinomialNode* parent);

    /**
     * @brief Consolida arboles de igual grado en la lista de raices.
     */
    void consolidate();

    /**
     * @brief Actualiza el puntero al elemento minimo de la cola.
     */
    void update_min();

    /**
     * @brief Libera recursivamente los nodos de un arbol binomial.
     *
     * @param node Raiz del arbol o subarbol a liberar.
     */
    void delete_tree(BinomialNode* node);
};

#endif // BINOMIAL_HEAP_HPP