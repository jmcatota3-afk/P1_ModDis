#include "ListaCliente.h"

ListaCliente::ListaCliente() : cabeza(nullptr), ultimo(nullptr), cantidad(0)
{
}

ListaCliente::~ListaCliente()
{
   limpiar();
}

bool ListaCliente::agregar(Cliente* c)
{
   if (c == nullptr || contiene(c->getIdCliente()))
   {
      return false;
   }
   NodoCliente* nuevo = new NodoCliente(c);
   if (cabeza == nullptr)
   {
      cabeza = nuevo;
   }
   else
   {
      ultimo->setSiguiente(nuevo);
   }
   ultimo = nuevo;
   cantidad++;
   return true;
}

Cliente* ListaCliente::buscar(std::string idCliente)
{
   NodoCliente* actual = cabeza;
   while (actual != nullptr)
   {
      if (actual->getDato()->getIdCliente() == idCliente)
      {
         return actual->getDato();
      }
      actual = actual->getSiguiente();
   }
   return nullptr;
}

bool ListaCliente::contiene(std::string idCliente)
{
   return buscar(idCliente) != nullptr;
}

NodoCliente* ListaCliente::inicio()
{
   return cabeza;
}

int ListaCliente::tamano()
{
   return cantidad;
}

bool ListaCliente::estaVacia()
{
   return cabeza == nullptr;
}

void ListaCliente::limpiar()
{
   NodoCliente* actual = cabeza;
   while (actual != nullptr)
   {
      NodoCliente* siguiente = actual->getSiguiente();
      delete actual->getDato();     // la lista es duena de los clientes
      delete actual;
      actual = siguiente;
   }
   cabeza = nullptr;
   ultimo = nullptr;
   cantidad = 0;
}
