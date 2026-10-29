/***********************************************************************
 * Module:  ListaSimple.h
 * Purpose: Lista simplemente enlazada (procesadas y conjuntos A, B y C)
 * Comment: Guarda punteros a Transaccion. eliminar() y limpiar() liberan
 *          nodos, NO transacciones. No se puede copiar.
 ***********************************************************************/

#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

#include <string>
#include "Nodo.h"
#include "Transaccion.h"

class ListaSimple
{
public:
   ListaSimple();
   ~ListaSimple();

   void agregar(Transaccion* t);
   Transaccion* buscar(std::string id);
   bool contiene(std::string id);
   bool eliminar(std::string id);
   Nodo* inicio();
   int tamano();
   bool estaVacia();
   void limpiar();

private:
   Nodo* cabeza;
   Nodo* ultimo;
   int cantidad;
};

#endif