#include "binomial_heap.hpp"

#include <stdexcept>
#include <utility>

BinomialHeap::BinomialHeap()
    : head_(nullptr),
      min_(nullptr),
      size_(0) {
}

BinomialHeap::~BinomialHeap() {
    BinomialNode* current = head_;

    while (current != nullptr) {
        BinomialNode* next = current->sibling;
        current->sibling = nullptr;
        delete_tree(current);
        current = next;
    }
}

void BinomialHeap::build(const std::vector<double>& keys) {
    if (head_ != nullptr) {
        throw std::logic_error(
            "La cola binomial debe estar vacia antes de construirla"
        );
    }

    node_by_vertex_.resize(keys.size(), nullptr);

    std::vector<BinomialNode*> trees;

    for (int vertex = 0; vertex < static_cast<int>(keys.size()); ++vertex) {
        BinomialNode* node = new BinomialNode{
            keys[vertex],
            vertex,
            0,
            nullptr,
            nullptr,
            nullptr
        };

        node_by_vertex_[vertex] = node;
        size_++;

        BinomialNode* current = node;
        int degree = 0;

        while (degree < static_cast<int>(trees.size()) &&
               trees[degree] != nullptr) {
            BinomialNode* other = trees[degree];
            trees[degree] = nullptr;

            if (current->key <= other->key) {
                link(other, current);
            } else {
                link(current, other);
                current = other;
            }

            degree++;
        }

        if (degree == static_cast<int>(trees.size())) {
            trees.push_back(current);
        } else {
            trees[degree] = current;
        }
    }

    head_ = nullptr;
    BinomialNode* previous = nullptr;

    for (BinomialNode* tree : trees) {
        if (tree == nullptr) {
            continue;
        }

        if (head_ == nullptr) {
            head_ = tree;
        } else {
            previous->sibling = tree;
        }

        previous = tree;
    }

    update_min();
}

void BinomialHeap::link(BinomialNode* child, BinomialNode* parent) {
    child->parent = parent;
    child->sibling = parent->child;
    parent->child = child;
    parent->degree++;
}

void BinomialHeap::consolidate() {
    if (head_ == nullptr) {
        return;
    }

    BinomialNode* previous = nullptr;
    BinomialNode* current = head_;
    BinomialNode* next = current->sibling;

    while (next != nullptr) {
        bool different_degree =
            current->degree != next->degree;

        bool three_same_degrees =
            next->sibling != nullptr &&
            next->sibling->degree == current->degree;

        if (different_degree || three_same_degrees) {
            previous = current;
            current = next;
        } else if (current->key <= next->key) {
            current->sibling = next->sibling;
            link(next, current);
        } else {
            if (previous == nullptr) {
                head_ = next;
            } else {
                previous->sibling = next;
            }

            link(current, next);
            current = next;
        }

        next = current->sibling;
    }

    update_min();
}

int BinomialHeap::extract_min() {
    if (min_ == nullptr) {
        throw std::logic_error(
            "No se puede extraer el minimo de una cola vacia"
        );
    }

    BinomialNode* minimum = min_;

    // Buscar la raiz anterior al minimo para poder removerlo
    // de la lista de raices.
    BinomialNode* previous_min = nullptr;
    BinomialNode* current = head_;

    while (current != minimum) {
        previous_min = current;
        current = current->sibling;
    }

    if (previous_min == nullptr) {
        head_ = minimum->sibling;
    } else {
        previous_min->sibling = minimum->sibling;
    }

    // Invertir la lista de hijos del minimo.
    //
    // Los hijos aparecen originalmente en orden decreciente
    // de grado. Al invertirlos quedan en orden creciente,
    // como requiere la lista de raices.
    BinomialNode* child = minimum->child;
    BinomialNode* reversed_children = nullptr;

    while (child != nullptr) {
        BinomialNode* next = child->sibling;

        child->parent = nullptr;
        child->sibling = reversed_children;
        reversed_children = child;

        child = next;
    }

    // Unir la lista de hijos con la lista actual de raices.
    if (reversed_children != nullptr) {
        BinomialNode* last_child = reversed_children;

        while (last_child->sibling != nullptr) {
            last_child = last_child->sibling;
        }

        last_child->sibling = head_;
        head_ = reversed_children;
    }

    int minimum_vertex = minimum->vertex;

    node_by_vertex_[minimum_vertex] = nullptr;

    delete minimum;
    size_--;

    if (head_ == nullptr) {
        min_ = nullptr;
    } else {
        consolidate();
    }

    return minimum_vertex;
}

void BinomialHeap::update_min() {
    min_ = nullptr;

    BinomialNode* current = head_;

    while (current != nullptr) {
        if (min_ == nullptr || current->key < min_->key) {
            min_ = current;
        }

        current = current->sibling;
    }
}

bool BinomialHeap::empty() const {
    return size_ == 0;
}

void BinomialHeap::delete_tree(BinomialNode* node) {
    if (node == nullptr) {
        return;
    }

    BinomialNode* child = node->child;

    while (child != nullptr) {
        BinomialNode* next = child->sibling;
        child->sibling = nullptr;
        delete_tree(child);
        child = next;
    }

    delete node;
}

void BinomialHeap::decrease_key(int vertex, double new_key) {
    if (vertex < 0 ||
        vertex >= static_cast<int>(node_by_vertex_.size())) {
        throw std::out_of_range("Vertice fuera de rango");
    }

    BinomialNode* node = node_by_vertex_[vertex];

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

    BinomialNode* current = node;
    BinomialNode* parent = current->parent;

    while (parent != nullptr &&
           current->key < parent->key) {

        std::swap(current->key, parent->key);
        std::swap(current->vertex, parent->vertex);

        node_by_vertex_[current->vertex] = current;
        node_by_vertex_[parent->vertex] = parent;

        current = parent;
        parent = current->parent;
    }

    update_min();
}

bool BinomialHeap::contains(int vertex) const {
    if (vertex < 0 ||
        vertex >= static_cast<int>(node_by_vertex_.size())) {
        return false;
    }

    return node_by_vertex_[vertex] != nullptr;
}