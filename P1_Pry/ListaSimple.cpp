/***********************************************************************
 * Module:  ListaSimple.cpp
 * Purpose: Implementación de la lista simplemente enlazada para transacciones
 *****************************************************************──────*/

#include "ListaSimple.h"

ListaSimple::ListaSimple() : cabeza(nullptr), ultimo(nullptr), cantidad(0) {}

ListaSimple::~ListaSimple() {
    limpiar();
}

void ListaSimple::agregar(Transaccion* t) {
    Nodo* nuevo = new Nodo(t);
    if (estaVacia()) {
        cabeza = nuevo;
        ultimo = nuevo;
    } else {
        ultimo->setSiguiente(nuevo);
        ultimo = nuevo;
    }
    cantidad++;
}

Transaccion* ListaSimple::buscar(std::string id) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->getDato() != nullptr && actual->getDato()->getIdT() == id) {
            return actual->getDato();
        }
        actual = actual->getSiguiente();
    }
    return nullptr;
}

bool ListaSimple::contiene(std::string id) {
    return buscar(id) != nullptr;
}

bool ListaSimple::eliminar(std::string id) {
    if (estaVacia()) {
        return false;
    }

    Nodo* actual = cabeza;
    Nodo* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->getDato() != nullptr && actual->getDato()->getIdT() == id) {
            if (anterior == nullptr) {
                cabeza = actual->getSiguiente();
                if (cabeza == nullptr) {
                    ultimo = nullptr;
                }
            } else {
                anterior->setSiguiente(actual->getSiguiente());
                if (actual == ultimo) {
                    ultimo = anterior;
                }
            }
            delete actual;      // solo el nodo, NO la transaccion
            cantidad--;
            return true;
        }
        anterior = actual;
        actual = actual->getSiguiente();
    }
    return false;
}

Nodo* ListaSimple::inicio() {
    return cabeza;
}

int ListaSimple::tamano() {
    return cantidad;
}

bool ListaSimple::estaVacia() {
    return cabeza == nullptr;
}

void ListaSimple::limpiar() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->getSiguiente();
        delete actual;          // solo el nodo, NO la transaccion
        actual = siguiente;
    }
    cabeza = nullptr;
    ultimo = nullptr;
    cantidad = 0;
}