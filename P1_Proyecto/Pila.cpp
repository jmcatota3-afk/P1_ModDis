#include "Pila.h"

Pila::Pila() : cima(nullptr), cantidad(0)
{
}

Pila::~Pila()
{
   limpiar();
}

void Pila::push(Transaccion* t)
{
   //Declaro el nodo o dato 
   Nodo* nuevo = new Nodo(t);
   nuevo ->setSiguiente(cima);
   cima = nuevo;
   cantidad++; 
}

Transaccion* Pila::pop()
{
   if (cima == nullptr)
   return nullptr;
   Nodo* nodoSaliente = cima;
   Transaccion* dato = nodoSaliente->getDato();
   // la cima baja de un nivel o prioridad 
   cima = cima->getSiguiente();
   // se libera el nodo pero la trasaccion sigue ahi 
   delete nodoSaliente;
   cantidad--;
   return dato;
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
   while (cima != nullptr)
   pop();
   
}