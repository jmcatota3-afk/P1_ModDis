/***********************************************************************
 * Module:  Nodo.h
 * Purpose: Pieza basica de ListaSimple, Cola y Pila
 * Comment: Guarda un PUNTERO a Transaccion y un puntero al nodo siguiente.
 *          No es dueno de la transaccion ni del nodo siguiente: no los
 *          libera. De eso se encargan SistemaFraude y la estructura que
 *          contiene al nodo.
 ***********************************************************************/

#ifndef NODO_H
#define NODO_H

#include "Transaccion.h"

class Nodo
{
public:
   Nodo(Transaccion* dato);
   ~Nodo();

   Transaccion* getDato();
   Nodo* getSiguiente();
   void setSiguiente(Nodo* siguiente);

private:
   Transaccion* dato;
   Nodo* siguiente;
};

#endif