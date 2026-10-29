/***********************************************************************
 * Module:  ListaCliente.h
 * Purpose: Lista simplemente enlazada de clientes del banco
 * Comment: A diferencia de ListaSimple, esta lista SI es duena de los
 *          Cliente: los crea el sistema con new y los libera aqui.
 *          No se puede copiar.
 ***********************************************************************/

#ifndef LISTACLIENTE_H
#define LISTACLIENTE_H

#include <string>
#include "NodoCliente.h"
#include "Cliente.h"

class ListaCliente
{
public:
   ListaCliente();
   ~ListaCliente();
   ListaCliente(const ListaCliente& otra) = delete;
   ListaCliente& operator=(const ListaCliente& otra) = delete;

   bool agregar(Cliente* c);              // false si el ID ya existe
   Cliente* buscar(std::string idCliente);
   bool contiene(std::string idCliente);
   NodoCliente* inicio();
   int tamano();
   bool estaVacia();
   void limpiar();                        // libera nodos Y clientes

private:
   NodoCliente* cabeza;
   NodoCliente* ultimo;
   int cantidad;
};

#endif
