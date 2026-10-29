#include "NodoCliente.h"

NodoCliente::NodoCliente(Cliente* dato) : dato(dato), siguiente(nullptr)
{
}

NodoCliente::~NodoCliente()
{
}

Cliente* NodoCliente::getDato()
{
   return dato;
}

NodoCliente* NodoCliente::getSiguiente()
{
   return siguiente;
}

void NodoCliente::setSiguiente(NodoCliente* siguiente)
{
   this->siguiente = siguiente;
}

