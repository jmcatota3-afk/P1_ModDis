#include "Cola.h"

Cola::Cola() : frente(nullptr), fin(nullptr), cantidad(0)
{
}

Cola::~Cola()
{
   limpiar();
}

void Cola::encolar(Transaccion* t)
{
   // Llamo al nodo 
   Nodo* nuevo = new Nodo(t);
   // validaciones 
   if(fin == nullptr){
      frente = nuevo;
      fin = nuevo;
   }else {
      fin ->setSiguiente(nuevo); 
      fin = nuevo;
   }
   // cantidad de la cola 
   cantidad++;
}

Transaccion* Cola::desencolar()
{
   if(frente == nullptr)
   return nullptr;
   Nodo* nodoSaliente = frente;
   Transaccion* dato = nodoSaliente->getDato();
   // Caso la cola avanza 
   frente = frente->getSiguiente();
   // si ya no queda nada en la cola el final apunta a nullo
   if(frente == nullptr){
      fin = nullptr;
   } 
   // libreacion del nodo o dato 
   delete nodoSaliente;
   //restamos la cantidad de dato o nodo de la cola 
   cantidad--;
   // devuelve el dato 
   return dato;
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
   // limpia o elimina la cola 
   while(frente != nullptr){
      desencolar();
   }
}