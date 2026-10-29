#include "SistemaFraude.h"
#include <cctype>
using namespace std;
SistemaFraude::~SistemaFraude()
{
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
      delete actual->getDato();
      actual = actual->getSiguiente();
   }
   procesadas.limpiar();

   while (!pendientes.estaVacia())
   {
      delete pendientes.desencolar();
   }
}

bool SistemaFraude::registrarCliente(string idCliente,string nombre,string paisHabitual,double limiteDiario){
    if(clientes.contiene(idCliente)){
        return false; 
    } 
    clientes.agregar(new Cliente (idCliente,nombre,paisHabitual,limiteDiario));
    return true;
}
Cliente* SistemaFraude ::buscarCliente(string idCliente){
    return clientes.buscar(idCliente);
}
ListaCliente& SistemaFraude:: getClientes(){
    return clientes;
}

bool SistemaFraude:: ingresarTransaccion(string idCliente, double monto,string pais, string& idGenerado){
    if(!clientes.contiene(idCliente) || monto <=0){
        return false;
    }
    Transaccion* t = new Transaccion(idCliente,monto,pais);
    idGenerado = t->getIdT();
    pendientes.encolar(t);
    return true; 
}
bool SistemaFraude::cargarTransaccion(string id, string idCliente, double monto,string pais, time_t fechaHora,string estadoRevision)
{
   if (existeId(id) || !clientes.contiene(idCliente))
   {
      return false;
   }
   Transaccion* t = new Transaccion(id, idCliente, monto, pais, fechaHora);
   t->setEstadoRevision(estadoRevision);
   pendientes.encolar(t);
   return true;
}
Transaccion* SistemaFraude::buscarTransaccion(string idTransaccion)
{
   Transaccion* t = procesadas.buscar(idTransaccion);
   if (t != nullptr)
   {
      return t;
   }
   Nodo* actual = pendientes.inicio();
   while (actual != nullptr)
   {
      if (actual->getDato()->getIdT() == idTransaccion)
      {
         return actual->getDato();
      }
      actual = actual->getSiguiente();
   }
   return nullptr;
}
bool SistemaFraude::modificarTransaccion(string idTransaccion, double monto, string pais)
{
   Transaccion* t = buscarTransaccion(idTransaccion);
   if (t == nullptr || monto <= 0)
   {
      return false;
   }
   t->setMontoBancario(monto);
   t->setPaisTransaccion(pais);
   t->setEstadoRevision(ESTADO_PENDIENTE);
   reclasificarTodo();
   return true;
}
bool SistemaFraude::eliminarTransaccion(string idTransaccion)
{
   Transaccion* t = procesadas.buscar(idTransaccion);
   if (t != nullptr)
   {
      procesadas.eliminar(idTransaccion);
      delete t;
      reclasificarTodo();
      return true;
   }

   Cola temporal;
   bool eliminada = false;
   while (!pendientes.estaVacia())
   {
      Transaccion* pendiente = pendientes.desencolar();
      if (!eliminada && pendiente->getIdT() == idTransaccion)
      {
         delete pendiente;
         eliminada = true;
      }
      else
      {
         temporal.encolar(pendiente);
      }
   }
   while (!temporal.estaVacia())
   {
      pendientes.encolar(temporal.desencolar());
   }
   return eliminada;
}
int SistemaFraude::procesarCola()
{
   int contador = 0;
   while (!pendientes.estaVacia())
   {
      Transaccion* t = pendientes.desencolar();
      evaluar(t);
      procesadas.agregar(t);
      clasificar(t);
      contador++;
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
      evaluar(t);
      clasificar(t);
      actual = actual->getSiguiente();
   }
}
//CONJUNTOS 
ListaSimple& SistemaFraude::getConjuntoA() { 
    return conjuntoA;
 }
ListaSimple& SistemaFraude::getConjuntoB() { 
    return conjuntoB;
 }
ListaSimple& SistemaFraude::getConjuntoC() { 
    return conjuntoC;
}
void SistemaFraude::interseccionAB(ListaSimple& resultado)
{
   resultado.limpiar();
   Nodo* actual = conjuntoA.inicio();
   while (actual != nullptr)
   {
      Transaccion* t = actual->getDato();
      if (conjuntoB.contiene(t->getIdT()))
      {
         resultado.agregar(t);
      }
      actual = actual->getSiguiente();
   }
}

void SistemaFraude::unionBC(ListaSimple& resultado)
{
   resultado.limpiar();
   Nodo* actual = conjuntoB.inicio();
   while (actual != nullptr)
   {
      resultado.agregar(actual->getDato());
      actual = actual->getSiguiente();
   }
   actual = conjuntoC.inicio();
   while (actual != nullptr)
   {
      Transaccion* t = actual->getDato();
      if (!resultado.contiene(t->getIdT()))
      {
         resultado.agregar(t);
      }
      actual = actual->getSiguiente();
   }
}

void SistemaFraude::diferenciaAC(ListaSimple& resultado)
{
   resultado.limpiar();
   Nodo* actual = conjuntoA.inicio();
   while (actual != nullptr)
   {
      Transaccion* t = actual->getDato();
      if (!conjuntoC.contiene(t->getIdT()))
      {
         resultado.agregar(t);
      }
      actual = actual->getSiguiente();
   }
}
