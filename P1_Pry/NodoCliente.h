/***********************************************************************
 * Module:  NodoCliente.h
 * Purpose: Pieza basica de ListaCliente
 ***********************************************************************/

#ifndef NODOCLIENTE_H
#define NODOCLIENTE_H

#include "Cliente.h"

class NodoCliente
{
public:
   NodoCliente(Cliente* dato);
   ~NodoCliente();

   Cliente* getDato();
   NodoCliente* getSiguiente();
   void setSiguiente(NodoCliente* siguiente);

private:
   Cliente* dato;
   NodoCliente* siguiente;
};

#endif
