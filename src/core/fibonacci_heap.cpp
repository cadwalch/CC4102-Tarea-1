#include "fibonacci_heap.hpp"
#include <vector>
#include <stdexcept>

FibonacciHeap::FibonacciHeap()
    : min_(nullptr),
      size_(0),
      cut_count_(0),
      cascading_cut_count_(0) {
}

FibonacciHeap::~FibonacciHeap() {
    delete_list(min_);
}

void FibonacciHeap::build(
    const std::vector<double>& keys
) {
    if (min_ != nullptr) {
        throw std::logic_error(
            "La cola de Fibonacci debe estar vacia antes de construirla"
        );
    }

    node_by_vertex_.resize(keys.size(), nullptr);

    for (int vertex = 0;
         vertex < static_cast<int>(keys.size());
         ++vertex) {

        FibonacciNode* node = new FibonacciNode{
            keys[vertex],
            vertex,
            0,
            false,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };

        node->left = node;
        node->right = node;

        node_by_vertex_[vertex] = node;

        add_to_root_list(node);

        size_++;
    }
}

void FibonacciHeap::add_to_root_list(
    FibonacciNode* node
) {
    node->parent = nullptr;
    node->mark = false;

    if (min_ == nullptr) {
        node->left = node;
        node->right = node;

        min_ = node;

        return;
    }

    node->left = min_;
    node->right = min_->right;

    min_->right->left = node;
    min_->right = node;

    if (node->key < min_->key) {
        min_ = node;
    }
}

bool FibonacciHeap::contains(int vertex) const {
    if (vertex < 0 ||
        vertex >= static_cast<int>(node_by_vertex_.size())) {
        return false;
    }

    return node_by_vertex_[vertex] != nullptr;
}

bool FibonacciHeap::empty() const {
    return size_ == 0;
}

void FibonacciHeap::delete_list(
    FibonacciNode* node
) {
    if (node == nullptr) {
        return;
    }

    FibonacciNode* current = node->right;

    while (current != node) {
        FibonacciNode* next = current->right;

        if (current->child != nullptr) {
            delete_list(current->child);
        }

        delete current;
        current = next;
    }

    if (node->child != nullptr) {
        delete_list(node->child);
    }

    delete node;
}

void FibonacciHeap::link(
    FibonacciNode* child,
    FibonacciNode* parent
) {
    // Remover child de la lista de raices.
    child->left->right = child->right;
    child->right->left = child->left;

    child->parent = parent;
    child->mark = false;

    // Si parent no tiene hijos, child crea una nueva lista circular.
    if (parent->child == nullptr) {
        child->left = child;
        child->right = child;

        parent->child = child;
    } else {
        FibonacciNode* first_child = parent->child;

        child->left = first_child;
        child->right = first_child->right;

        first_child->right->left = child;
        first_child->right = child;
    }

    parent->degree++;
}

void FibonacciHeap::consolidate() {
    if (min_ == nullptr) {
        return;
    }

    std::vector<FibonacciNode*> roots;

    FibonacciNode* current = min_;

    do {
        roots.push_back(current);
        current = current->right;
    } while (current != min_);

    std::vector<FibonacciNode*> degree_table;

    for (FibonacciNode* root : roots) {
        // Puede haber dejado de ser raiz durante una consolidacion anterior.
        if (root->parent != nullptr) {
            continue;
        }

        FibonacciNode* x = root;
        int degree = x->degree;

        while (degree >= static_cast<int>(degree_table.size())) {
            degree_table.push_back(nullptr);
        }

        while (degree_table[degree] != nullptr) {
            FibonacciNode* y = degree_table[degree];

            if (y->key < x->key) {
                FibonacciNode* temp = x;
                x = y;
                y = temp;
            }

            link(y, x);

            degree_table[degree] = nullptr;

            degree++;

            while (degree >=
                   static_cast<int>(degree_table.size())) {
                degree_table.push_back(nullptr);
            }
        }

        degree_table[degree] = x;
    }

    min_ = nullptr;

    for (FibonacciNode* root : degree_table) {
        if (root == nullptr) {
            continue;
        }

        root->left = root;
        root->right = root;

        add_to_root_list(root);
    }
}

int FibonacciHeap::extract_min() {
    if (min_ == nullptr) {
        throw std::logic_error(
            "No se puede extraer el minimo de una cola vacia"
        );
    }

    FibonacciNode* minimum = min_;

    // Guardar los hijos antes de modificar la lista de raices.
    std::vector<FibonacciNode*> children;

    if (minimum->child != nullptr) {
        FibonacciNode* current = minimum->child;

        do {
            children.push_back(current);
            current = current->right;
        } while (current != minimum->child);
    }

    // Remover cada hijo de su lista y agregarlo como raiz.
    for (FibonacciNode* child : children) {
        child->left->right = child->right;
        child->right->left = child->left;

        child->left = child;
        child->right = child;
        child->parent = nullptr;
        child->mark = false;

        add_to_root_list(child);
    }

    minimum->child = nullptr;

    // Determinar si minimum era la unica raiz.
    bool only_root =
        minimum->right == minimum;

    if (only_root) {
        min_ = nullptr;
    } else {
        FibonacciNode* next_root = minimum->right;

        minimum->left->right = minimum->right;
        minimum->right->left = minimum->left;

        min_ = next_root;
    }

    int minimum_vertex = minimum->vertex;

    node_by_vertex_[minimum_vertex] = nullptr;

    delete minimum;
    size_--;

    if (min_ != nullptr) {
        consolidate();
    }

    return minimum_vertex;
}

void FibonacciHeap::cut(
    FibonacciNode* node,
    FibonacciNode* parent
) {
    // Si node es el hijo apuntado directamente por parent,
    // actualizar parent->child.
    if (parent->child == node) {
        if (node->right == node) {
            parent->child = nullptr;
        } else {
            parent->child = node->right;
        }
    }

    // Remover node de la lista circular de hijos.
    node->left->right = node->right;
    node->right->left = node->left;

    parent->degree--;
    cut_count_++;
    // Dejar node aislado antes de moverlo a las raices.
    node->left = node;
    node->right = node;
    node->parent = nullptr;
    node->mark = false;

    add_to_root_list(node);
}

void FibonacciHeap::cascading_cut(
    FibonacciNode* node
) {
    FibonacciNode* parent = node->parent;

    if (parent == nullptr) {
        return;
    }

    if (!node->mark) {
        node->mark = true;
    } else {
        cascading_cut_count_++;

        cut(node, parent);
        cascading_cut(parent);
    }
}

long long FibonacciHeap::cut_count() const {
    return cut_count_;
}

long long FibonacciHeap::cascading_cut_count() const {
    return cascading_cut_count_;
}

void FibonacciHeap::decrease_key(
    int vertex,
    double new_key
) {
    if (vertex < 0 ||
        vertex >= static_cast<int>(node_by_vertex_.size())) {
        throw std::out_of_range(
            "Vertice fuera de rango"
        );
    }

    FibonacciNode* node =
        node_by_vertex_[vertex];

    if (node == nullptr) {
        throw std::logic_error(
            "El vertice no se encuentra en la cola"
        );
    }

    if (new_key > node->key) {
        throw std::invalid_argument(
            "La nueva clave no puede ser mayor que la actual"
        );
    }

    node->key = new_key;

    FibonacciNode* parent = node->parent;

    if (parent != nullptr &&
        node->key < parent->key) {

        cut(node, parent);
        cascading_cut(parent);
    }

    if (node->key < min_->key) {
        min_ = node;
    }
}