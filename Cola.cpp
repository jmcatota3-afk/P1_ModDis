#include "Cola.h"

Cola::Cola() : frente(nullptr), fin(nullptr), cantidad(0)
{
}

Cola::~Cola()
{
   // TODO : implement
}

void Cola::encolar(Transaccion* t)
{
   // TODO : implement
}

Transaccion* Cola::desencolar()
{
   // TODO : implement
   return nullptr;
}

Nodo* Cola::inicio()
{
   return frente;
}

int Cola::tamano()
{
   return cantidad;
}

bool Cola::estaVacia()
{
   return cantidad == 0;
}

void Cola::limpiar()
{
   // TODO : implement
}