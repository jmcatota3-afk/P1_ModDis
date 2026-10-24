#include "ListaSimple.h"

ListaSimple::ListaSimple() : cabeza(nullptr), ultimo(nullptr), cantidad(0)
{
}

ListaSimple::~ListaSimple()
{
   // TODO : implement
}

void ListaSimple::agregar(Transaccion* t)
{
  printf("hola");
}

Transaccion* ListaSimple::buscar(std::string id)
{
   // TODO : implement
   return nullptr;
}

bool ListaSimple::contiene(std::string id)
{
   // TODO : implement
   return false;
}

bool ListaSimple::eliminar(std::string id)
{
   // TODO : implement
   return false;
}

Nodo* ListaSimple::inicio()
{
   return cabeza;
}

int ListaSimple::tamano()
{
   return cantidad;
}

bool ListaSimple::estaVacia()
{
   return cantidad == 0;
}

void ListaSimple::limpiar()
{
   // TODO : implement
}