/*
 * Nombre del Archivo: SplayTree.h
 * 
 * Descripción General:
 * 
 * Esta es una clase que implementa la estructura de datos SplayTree, 
 * el cual funciona como un árbol binario que realiza la operación 
 * Splay (traer nodo a la raíz) cada que se hace una insersión, búsqueda,
 * un borrado. Splay trae con sigo el nodo ya sea insertado, borrado o 
 * buscado junto con sus hijos a la raíz de esta manera quedan "cerca"
 * los nodos más cercanos a este.
 * 
 * Autor: Mauricio Avilés y Josué Hidalgo
 * 
 */

#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <stdexcept>
#include <iostream>

#include "SNode.h"
#include "DLinkedList.h"

using std::cout;
using std::endl;
using std::runtime_error;

template <typename E>
class SplayTree {
private:
    SNode<E> *root;
    SNode<E> *last; 

    SNode<E>* insertAux(SNode<E> *current, E element) {
        if (current == nullptr) { 
            SNode<E> *result = new SNode<E>(element);
            last = result; 
            return result;
        }
        if (element == current->element) {
            last = current;
            throw runtime_error("Duplicated element.");
        }
        if (element < current->element) {
            current->left = insertAux(current->left, element);
            current->left->parent = current;
            return current;
        } else {
            current->right = insertAux(current->right, element);
            current->right->parent = current;
            return current;
        }
    }

    E findAux(SNode<E> *current, E element) {
        if (current == nullptr)
            throw runtime_error("Element not found.");
        last = current;
        if (element == current->element) {
            return current->element;
        }
        if (element < current->element)
            return findAux(current->left, element);
        else
            return findAux(current->right, element);
    }

    E* findPointerAux(SNode<E> *current, E element) {
        if (current == nullptr)
            throw runtime_error("Element not found.");
        last = current;
        if (element == current->element)
            return &(current->element);
        if (element < current->element)
            return findPointerAux(current->left, element);
        else
            return findPointerAux(current->right, element);
    }

    bool containsAux(SNode<E> *current, E element) {
        // Caso Base
        if (current == nullptr)
            return false;
        last = current;
        // Caso Base
        if (element == current->element)
            return true;
        // 
        if (element < current->element)
            return containsAux(current->left, element);
        else
            return containsAux(current->right, element);
    }

    SNode<E>* removeAux(SNode<E>* current, E element, E* result) {
        if (current == nullptr)
            throw runtime_error("Element not found.");

        // Actualiza el last al nodo actual
        last = current;

        // Casos Base

        // ElementoBuscar < Actual
        if (element < current->element) {
            current->left = removeAux(current->left, element, result);
            if (current->left != nullptr)
                current->left->parent = current; // Actualiza el parent
            return current;
        }
        // ElementoBuscar > Actual
        if (element > current->element) {
            current->right = removeAux(current->right, element, result);
            if (current->right != nullptr)
                current->right->parent = current; // Actualiza el parent
            return current;
        }
        // ElementoBuscar == Actual
        *result = current->element;
        last = current->parent;

        // ¿Tiene Hijos?

        // No tiene hijos
        if (current->childrenCount() == 0) {
            delete current;
            return nullptr;
        }
        // Tiene un solo hijo
        if (current->childrenCount() == 1) {
            SNode<E>* onlyChild = current->getOnlyChild();
            if (onlyChild != nullptr)
                onlyChild->parent = current->parent; // Actualiza el parent del hijo único
            delete current;
            return onlyChild;
        }
        // Tiene dos hijos: buscar sucesor, intercambiar y eliminar sucesor
        SNode<E>* successor = getSuccessor(current);
        swap(current, successor);
        current->right = removeAux(current->right, element, result);
        if (current->right != nullptr)
            current->right->parent = current; // Actualiza el parent del nuevo hijo derecho
        return current;
    }

    SNode<E> *getSuccessor(SNode<E> *current) {
        SNode<E> *successor = current->right;
        while (successor->left != nullptr)
            successor = successor->left;
        return successor;
    }

    void swap(SNode<E> *n1, SNode<E> *n2) {
        E temp = n1->element;
        n1->element = n2->element;
        n2->element = temp;
    }

    void clearAux(SNode<E> *current) {
        if (current == nullptr)
            return;
        clearAux(current->left);
        clearAux(current->right);
        delete current;
    }

    void getElementsAux(SNode<E> *current, List<E> *elements) {
        if (current == nullptr)
            return;
        getElementsAux(current->left, elements);
        elements->append(current->element);
        getElementsAux(current->right, elements);
    }

    int getSizeAux(SNode<E> *current) {
        if (current == nullptr)
            return 0;
        return 1 + getSizeAux(current->left) + getSizeAux(current->right);
    }

    int height(SNode<E> *current) {
        if (current == nullptr)
            return 0;
        int lh = height(current->left);
        int rh = height(current->right);
        return 1 + (lh > rh? lh : rh);
    }

    void rotateRight(SNode<E> *current) {
        // Tiene que existir raíz y hijo izquierdo
        if (current == nullptr)
            throw runtime_error("Can't rotate right a null tree.");
        if (current->left == nullptr)
            throw runtime_error("Can't rotate right with a null left child.");

        // Current apunta al nodo que ahora será hijo
        // Temp apunta al nodo que será el papá (el más arriba)
        SNode<E> *temp = current->left;
        
        //Intercambio de Papás
        temp->parent = current->parent;

        // Mi segundo nieto ahora será mi hijo
        current->left = temp->right;
        // Si exite ese nieto él me dirá papá
        if (current->left != nullptr)
            current->left->parent = current;

        // El nuevo papá reconoce a su antiguo papá como hijo
        temp->right = current;
        
        //Intercambio de Papás
        current->parent = temp;

        // Si el que ANTES era papa era Papá Supremo ahora lo será el nuevo papá (temp)
        // Sino si mi papá esta apuntando por uno u otro lado al que ahora es mi hijo
        // entonces reconozcame como su nuevo hijo.
        
        // Current no tiene papá
        if (current == root)
            root = temp;
        // Current tenía un papá
        else if (temp->parent->right == current)
            temp->parent->right = temp;
        else
            temp->parent->left = temp;
    }

    void rotateLeft(SNode<E> *current) {
        if (current == nullptr)
            throw runtime_error("Can't rotate right a null tree.");
        if (current->right == nullptr)
            throw runtime_error("Can't rotate right with a null right child.");

        // Current apunta al nodo que ahora será hijo
        // Temp apunta al nodo que será el papá (el más arriba)
        SNode<E>* temp = current->right;
        
        //Intercambio de Papás
        temp->parent = current->parent;

        // Mi primer nieto ahora será mi segundo hijo
        current->right = temp->left;
        // Si exite ese nieto él me dirá papá
        if (current->right != nullptr)
            current->right->parent = current;

        // El nuevo papá reconoce a su antiguo papá como hijo
        temp->left = current;

        //Intercambio de Papás
        current->parent = temp;
        
        // Si el que ANTES era papa era Papá Supremo ahora lo será el nuevo papá (temp)
        // Sino si mi papá esta apuntando por uno u otro lado al que ahora es mi hijo
        // entonces reconozcame como su nuevo hijo.
        if (current == root)
            root = temp;
        else if (temp->parent->left == current)
            temp->parent->left = temp;
        else
            temp->parent->right = temp;
    }

    void splay() {
        // Last ya esta en la raíz
        if (last == nullptr || last == root)
            return;
        // Hasta que last esté en la raíz
        while (last != root) {
            // Si es hijo
            if (root->right == last) {
                rotateLeft(root);
                return; 
            }
            else if (root->left == last) {
                rotateRight(root);
                return;
            }
            // Si es Nieto
                // Busque el abuelo
            SNode<E>* grand = last->parent->parent;
			SNode<E>* parent = last->parent;
            // Casos de la izquierda
            if (parent == grand->left) {
                // L-L
                if (last == parent->left) {
                    rotateRight(grand);
                    rotateRight(parent);
                }
                // L-R
                else {
                    rotateLeft(parent);
                    rotateRight(grand);
                }
            }
            // Casos de la derecha
            else {
                // R-R
                if (last == parent->right) {
                    rotateLeft(grand);
                    rotateLeft(parent);
                }
                // R-L
                else {
                    rotateRight(parent);
                    rotateLeft(grand);
                }
            }
        }
    }

public:
    SplayTree() {
        root = nullptr;
        last = nullptr;
    }
    ~SplayTree() {
        clear();
    }

    void insert(E element) {
        try {
            root = insertAux(root, element);
            root->parent = nullptr;
            splay();
        }
        catch (runtime_error &e) {
            splay();
            throw e;
        }
    }

    E find(E element) {
        try {
            E result = findAux(root, element);
            splay();
            return result;
        }
        catch (runtime_error &e) {
            splay();
            throw e;
        }
    }

    E* findPointer(E element) {
        try {
            E *result = findPointerAux(root, element);
            splay();
            return result;
        }
        catch (runtime_error &e) {
            splay();
            throw e;
        }
    }

    bool contains(E element) {
        bool result = containsAux(root, element);
        splay();
        return result;
    }

    E remove(E element) {
        try {
            E result;
            root = removeAux(root, element, &result);
            if (root != nullptr) root->parent = nullptr;
            splay();
            return result;
        }
        catch (runtime_error &e) {
            splay();
            throw e;
        }
    }

    void clear() {
        clearAux(root);
        root = nullptr;
    }

    List<E> *getElements() {
        List<E> *elements = new DLinkedList<E>();
        getElementsAux(root, elements);
        return elements;
    }

    int getSize() {
        return getSizeAux(root);
    }

    int getHeight() {
        return height(root);
    }

    void print() {
		if (root == nullptr) {
			cout << "Tree's empty." << endl;
			return;
		}
        List<E>* elements = getElements();
        elements->print();
        delete elements;
    }
};

#endif // SPLAYTREE_H
