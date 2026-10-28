#ifndef COLA_H
#define COLA_H

#include "Nodo.h"
#include "Transaccion.h"

class Cola
{
public:
   Cola();
   ~Cola();
   Cola(const Cola& otra) = delete;
   Cola& operator=(const Cola& otra) = delete;

   void encolar(Transaccion* t);
   Transaccion* desencolar();          // nullptr si esta vacia
   Nodo* inicio();
   int tamano();
   bool estaVacia();
   void limpiar();

private:
   Nodo* frente;
   Nodo* fin;
   int cantidad;
};

#endif