#include "Pila.h"

Pila::Pila() : cima(nullptr), cantidad(0)
{
}

Pila::~Pila()
{
   // TODO : implement
}

void Pila::push(Transaccion* t)
{
   // TODO : implement
}

Transaccion* Pila::pop()
{
   // TODO : implement
   return nullptr;
}

Transaccion* Pila::tope()
{
   return cima == nullptr ? nullptr : cima->getDato();
}

int Pila::tamano()
{
   return cantidad;
}

bool Pila::estaVacia()
{
   return cantidad == 0;
}

void Pila::limpiar()
{
   // TODO : implement
}