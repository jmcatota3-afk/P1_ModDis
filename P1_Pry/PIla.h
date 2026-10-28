#ifndef PILA_H
#define PILA_H

#include "Nodo.h"
#include "Transaccion.h"

class Pila
{
public:
   Pila();
   ~Pila();
   Pila(const Pila& otra) = delete;
   Pila& operator=(const Pila& otra) = delete;

   void push(Transaccion* t);
   Transaccion* pop();                 // nullptr si esta vacia
   Transaccion* tope();                // nullptr si esta vacia
   int tamano();
   bool estaVacia();
   void limpiar();

private:
   Nodo* cima;
   int cantidad;
};

#endif