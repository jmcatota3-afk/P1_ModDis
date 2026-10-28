#include "Nodo.h"

// Constructor 
Nodo::Nodo(Transaccion* dato) : dato(dato), siguiente(nullptr)
{
}
// Destructor 
Nodo::~Nodo()
{
}
// devueleve el dato
Transaccion* Nodo::getDato()
{
   return dato;
}
// devuelve el dato siguiente 
Nodo* Nodo::getSiguiente()
{
   return siguiente;
}

void Nodo::setSiguiente(Nodo* siguiente)
{
   this->siguiente = siguiente;
}