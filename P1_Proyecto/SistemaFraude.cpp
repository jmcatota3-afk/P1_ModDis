/***********************************************************************
 * Module:  SistemaFraude.cpp
 * Purpose: Implementación sincronizada con Transaccion, ListaSimple y Pila
 ***********************************************************************/

#include "SistemaFraude.h"
#include "Transaccion.h"
#include <cmath>
#include <iostream>

SistemaFraude::SistemaFraude()
   : limiteDiario(5000.0), ventanaMin(5), maxTx(3)
{
}

SistemaFraude::~SistemaFraude()
{
   // SistemaFraude es el único dueño de las transacciones: las libera en el destructor.
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       delete actual->getDato();
       actual = actual->getSiguiente();
   }
   procesadas.limpiar();

   while (!pendientes.estaVacia())
   {
       Transaccion* t = pendientes.desencolar();
       delete t;
   }
}

bool SistemaFraude::ingresarTransaccion(std::string idTransaccion, std::string idCliente, std::string nombreCliente, double montoBancario, std::string pais, int tiempoTransaccion)
{
   if (existeId(idTransaccion))
   {
       return false;
   }
   Transaccion* t = new Transaccion(idTransaccion, idCliente, nombreCliente, montoBancario, pais, tiempoTransaccion);
   pendientes.encolar(t);
   return true;
}

Transaccion* SistemaFraude::buscarTransaccion(std::string idTransaccion)
{
   return procesadas.buscar(idTransaccion);
}

bool SistemaFraude::modificarTransaccion(std::string idTransaccion, double montoBancario, std::string pais, int tiempoTransaccion)
{
   Transaccion* t = buscarTransaccion(idTransaccion);
   if (t == nullptr) return false;
   
   t->setMontoBancario(montoBancario);
   t->setPais(pais);
   t->setTiempoTransaccion(tiempoTransaccion);
   
   reclasificarTodo();
   return true;
}

bool SistemaFraude::eliminarTransaccion(std::string idTransaccion)
{
   Transaccion* t = procesadas.buscar(idTransaccion);
   if (t != nullptr)
   {
       procesadas.eliminar(idTransaccion);
       conjuntoA.eliminar(idTransaccion);
       conjuntoB.eliminar(idTransaccion);
       conjuntoC.eliminar(idTransaccion);
       delete t;
       return true;
   }
   return false;
}

int SistemaFraude::procesarCola()
{
   int contador = 0;
   while (!pendientes.estaVacia())
   {
       Transaccion* t = pendientes.desencolar();
       if (t != nullptr)
       {
           evaluar(t);
           procesadas.agregar(t);
           clasificar(t);
           contador++;
       }
   }
   return contador;
}

void SistemaFraude::reclasificarTodo()
{
   vaciarClasificacion();
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr)
       {
           evaluar(t);
           clasificar(t);
       }
       actual = actual->getSiguiente();
   }
}

ListaSimple& SistemaFraude::getConjuntoA() { 
    return conjuntoA;
 }
ListaSimple& SistemaFraude::getConjuntoB() { 
    return conjuntoB;
 }
ListaSimple& SistemaFraude::getConjuntoC() { 
    return conjuntoC;
 }

// CONJUNTOS 
void SistemaFraude::interseccionAB(ListaSimple& resultado)
{
   resultado.limpiar();
   Nodo* actual = conjuntoA.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr && conjuntoB.contiene(t->getId()))
       {
           resultado.agregar(t);
       }
       actual = actual->getSiguiente();
   }
}

void SistemaFraude::unionBC(ListaSimple& resultado)
{
   resultado.limpiar();
   Nodo* actualB = conjuntoB.inicio();
   while (actualB != nullptr)
   {
       Transaccion* t = actualB->getDato();
       if (t != nullptr) resultado.agregar(t);
       actualB = actualB->getSiguiente();
   }
   Nodo* actualC = conjuntoC.inicio();
   while (actualC != nullptr)
   {
       Transaccion* t = actualC->getDato();
       if (t != nullptr && !resultado.contiene(t->getId()))
       {
           resultado.agregar(t);
       }
       actualC = actualC->getSiguiente();
   }
}

void SistemaFraude::diferenciaAC(ListaSimple& resultado)
{
   resultado.limpiar();
   Nodo* actual = conjuntoA.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr && !conjuntoC.contiene(t->getId()))
       {
           resultado.agregar(t);
       }
       actual = actual->getSiguiente();
   }
}

Transaccion* SistemaFraude::verSiguienteAlerta()
{
   return alertas.tope();
}

bool SistemaFraude::resolverSiguienteAlerta(bool esFraude)
{
   // Usando pop() en lugar de desapilar
   Transaccion* t = alertas.pop();
   if (t == nullptr) return false;
   return true;
}

int SistemaFraude::cantidadAlertas()
{
   return alertas.tamano();
}

int SistemaFraude::totalProcesadas()
{
   return procesadas.tamano();
}

int SistemaFraude::totalPorNivel(std::string nivel)
{
   int count = 0;
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr && t->getNivel() == nivel)
       {
           count++;
       }
       actual = actual->getSiguiente();
   }
   return count;
}

int SistemaFraude::totalAlertas()
{
   ListaSimple unionTemp;
   unionBC(unionTemp);
   return unionTemp.tamano();
}

double SistemaFraude::porcentajeAlertas()
{
   int total = totalProcesadas();
   if (total == 0) return 0.0;
   return (static_cast<double>(totalAlertas()) / total) * 100.0;
}

ListaSimple& SistemaFraude::getProcesadas() { 
    return procesadas;
 }
Cola& SistemaFraude::getPendientes() { 
    return pendientes; 
}

// LEY INFERENCIA 

bool SistemaFraude::evaluarP(Transaccion* t)
{
   if (t == nullptr) return false;
   return t->getMontoBancario() > limiteDiario;
}

bool SistemaFraude::evaluarQ(Transaccion* t)
{
   if (t == nullptr) return false;
   std::string habitual = paisHabitual(t->getIdCliente(), t);
   return t->getPais() != habitual;
}

bool SistemaFraude::evaluarR(Transaccion* t)
{
   if (t == nullptr) return false;
   int contador = 0;
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       Transaccion* tx = actual->getDato();
       if (tx != nullptr && tx->getIdCliente() == t->getIdCliente())
       {
           if (std::abs(tx->getTiempoTransaccion() - t->getTiempoTransaccion()) <= ventanaMin)
           {
               contador++;
           }
       }
       actual = actual->getSiguiente();
   }
   return contador >= maxTx;
}

std::string SistemaFraude::inferir(bool P, bool Q, bool R){
   if(P && Q && R){
      return NIVEL_ALTO_RIESGO;
   }else if(P && Q){
      return NIVEL_SOSPECHOSA;
   }else if (!P && !Q){
      return NIVEL_NORMAL;
   }else {
      return NIVEL_INUSUAL;
   }
}

std::string SistemaFraude::describirRegla(bool P, bool Q, bool R)
{
   std::string nivel = inferir(P, Q, R);
   if(nivel == NIVEL_ALTO_RIESGO){
      return "Regla Aplicada P AND Q AND R 'La transacción es de alto riesgo.'";
   }else if (nivel == NIVEL_SOSPECHOSA){
      return "Regla Aplicada P AND Q 'La transacción es sospechosa.'";
   }else if (nivel == NIVEL_NORMAL){
      return "~P AND ~Q 'La transacción se considera normal.'";
   } else {
      return "La transacción se considera inusual";
   }
}

void SistemaFraude::evaluar(Transaccion* t)
{
   if (t == nullptr) return;
   bool p = evaluarP(t);
   bool q = evaluarQ(t);
   bool r = evaluarR(t);
   
   t->setP(p);
   t->setQ(q);
   t->setR(r);
   
   std::string nivel = inferir(p, q, r);
   t->setNivel(nivel);
   t->setRegla(describirRegla(p, q, r));
}

std::string SistemaFraude::paisHabitual(std::string idCliente, Transaccion* hasta)
{
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr && t->getIdCliente() == idCliente && t != hasta)
       {
           return t->getPais();
       }
       actual = actual->getSiguiente();
   }
   return hasta != nullptr ? hasta->getPais() : "";
}

void SistemaFraude::clasificar(Transaccion* t)
{
   if (t == nullptr) return;
   std::string nivel = t->getNivel();
   if (nivel == NIVEL_NORMAL)
   {
       conjuntoA.agregar(t);
   }
   else if (nivel == NIVEL_SOSPECHOSA)
   {
       conjuntoB.agregar(t);
       alertas.push(t); // Usando push en lugar de apilar
   }
   else if (nivel == NIVEL_ALTO_RIESGO)
   {
       conjuntoC.agregar(t);
       alertas.push(t); // Usando push en lugar de apilar
   }
   else
   {
       conjuntoB.agregar(t);
       alertas.push(t); // Usando push en lugar de apilar
   }
}

void SistemaFraude::vaciarClasificacion()
{
   conjuntoA.limpiar();
   conjuntoB.limpiar();
   conjuntoC.limpiar();
   while (!alertas.estaVacia())
   {
       alertas.pop(); // Usando pop en lugar de desapilar
   }
}

bool SistemaFraude::existeId(std::string idTransaccion)
{
   return procesadas.contiene(idTransaccion);
}
