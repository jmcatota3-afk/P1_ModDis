/***********************************************************************
 * Module:  SistemaFraude.cpp
 * Purpose: Implementación sincronizada con Transaccion, ListaSimple y Pila
 ***********************************************************************/

#include "SistemaFraude.h"
#include "Transaccion.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include <iomanip>
#include <algorithm>
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

// Genera identificadores secuenciales y reutiliza el ID de cliente si el nombre ya existe.
bool SistemaFraude::ingresarTransaccionAutomatica(std::string nombreCliente, double montoBancario, std::string pais, int tiempoTransaccion, std::string& idTransaccionGenerado, std::string& idClienteGenerado)
{
   int maxTransaccion = 0;
   int maxCliente = 0;
   bool clienteEncontrado = false;

   auto revisar = [&](Nodo* actual)
   {
      while (actual != nullptr)
      {
         Transaccion* t = actual->getDato();
         if (t != nullptr)
         {
            const std::string idT = t->getId();
            if (idT.size() > 1 && idT[0] == 'T')
            {
               try { maxTransaccion = std::max(maxTransaccion, std::stoi(idT.substr(1))); } catch (...) {}
            }
            const std::string idU = t->getIdCliente();
            if (idU.size() > 1 && idU[0] == 'U')
            {
               try { maxCliente = std::max(maxCliente, std::stoi(idU.substr(1))); } catch (...) {}
            }
            if (t->getNombreCliente() == nombreCliente && !clienteEncontrado)
            {
               idClienteGenerado = idU;
               clienteEncontrado = true;
            }
         }
         actual = actual->getSiguiente();
      }
   };

   revisar(procesadas.inicio());
   revisar(pendientes.inicio());

   if (!clienteEncontrado)
   {
      std::ostringstream id;
      id << "U" << std::setw(4) << std::setfill('0') << ++maxCliente;
      idClienteGenerado = id.str();
   }

   std::ostringstream id;
   id << "T" << std::setw(4) << std::setfill('0') << ++maxTransaccion;
   idTransaccionGenerado = id.str();

   return ingresarTransaccion(idTransaccionGenerado, idClienteGenerado, nombreCliente, montoBancario, pais, tiempoTransaccion);
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
       // La reclasificación reconstruye conjuntos y pila de alertas para no
       // dejar punteros a una transacción que ya fue eliminada.
       procesadas.eliminar(idTransaccion);
       delete t;
       reclasificarTodo();
       return true;
   }

   // También se permite eliminar una transacción que aún espera en la cola.
   // La cola no ofrece borrado por ID, así que se reconstruye conservando las demás.
   Cola temporal;
   bool eliminada = false;
   while (!pendientes.estaVacia())
   {
       Transaccion* pendiente = pendientes.desencolar();
       if (pendiente != nullptr && pendiente->getId() == idTransaccion && !eliminada)
       {
           delete pendiente;
           eliminada = true;
       }
       else if (pendiente != nullptr)
       {
           temporal.encolar(pendiente);
       }
   }
   while (!temporal.estaVacia()) pendientes.encolar(temporal.desencolar());
   return eliminada;
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
   // Se registra la decisión del analista antes de retirar la alerta de la pila.
   Transaccion* t = alertas.pop();
   if (t == nullptr) return false;
   t->setEstadoRevision(esFraude ? "fraude confirmado" : "falsa alarma");
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
   std::string paisActual = t->getPais();
   // Evita marcar como diferente un país solo por mayúsculas/minúsculas.
   for (char& c : habitual) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
   for (char& c : paisActual) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
   return paisActual != habitual;
}

bool SistemaFraude::evaluarR(Transaccion* t)
{
   if (t == nullptr) return false;
   // La transacción actual cuenta como una operación dentro de la ventana.
   int contador = 1;
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
   // R es verdadero si hay más de tres transacciones en una ventana de 5 min:
   // la actual más al menos tres transacciones previas (contador > maxTx).
   return contador > maxTx;
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
   std::cout << "DEBUG | ID cliente: " << t->getIdCliente()
          << " | Pais: " << t->getPais()
          << " | P: " << p
          << " | Q: " << q
          << " | R: " << r << '\n';
   
   t->setP(p);
   t->setQ(q);
   t->setR(r);
   
   std::string nivel = inferir(p, q, r);
   t->setNivel(nivel);
   t->setRegla(describirRegla(p, q, r));
}

std::string SistemaFraude::paisHabitual(std::string idCliente, Transaccion* hasta)
{
   // El país habitual se estima como el país más frecuente en el historial
   // ya procesado del cliente. Si no hay historial, la primera operación sirve
   // como referencia y no se marca por sí sola como geográficamente anómala.
   std::vector<std::pair<std::string, int>> frecuencias;
   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr && t->getIdCliente() == idCliente && t != hasta)
       {
           std::string pais = t->getPais();
           for (char& c : pais) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
           bool encontrado = false;
           for (auto& par : frecuencias)
           {
               if (par.first == pais) { ++par.second; encontrado = true; break; }
           }
           if (!encontrado) frecuencias.push_back({pais, 1});
       }
       actual = actual->getSiguiente();
   }
   if (frecuencias.empty()) return hasta != nullptr ? hasta->getPais() : "";
   std::string habitual = frecuencias.front().first;
   int mayor = frecuencias.front().second;
   for (const auto& par : frecuencias)
   {
       if (par.second > mayor) { habitual = par.first; mayor = par.second; }
   }
   return habitual;
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
   // El ID debe ser único tanto en las procesadas como en las pendientes.
   if (procesadas.contiene(idTransaccion)) return true;
   Nodo* actual = pendientes.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr && t->getId() == idTransaccion) return true;
       actual = actual->getSiguiente();
   }
   return false;
}
bool SistemaFraude::guardarArchivo()
{
   // Formato compatible con Archivo.cpp: un registro por línea y campos con '|'.
   std::ofstream archivo("transacciones.txt");
   if (!archivo.is_open()) return false;

   Nodo* actual = procesadas.inicio();
   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr)
       {
           archivo << t->getId() << "|" << t->getIdCliente() << "|"
                   << t->getNombreCliente() << "|" << t->getMontoBancario() << "|"
                   << t->getPais() << "|" << t->getTiempoTransaccion() << "|"
                   << t->getEstadoRevision() << "\n";
       }
       actual = actual->getSiguiente();
   }
   return archivo.good();
}

bool SistemaFraude::cargarArchivo()
{
   std::ifstream archivo("transacciones.txt");
   if (!archivo.is_open()) return false;

   std::string linea;
   while (std::getline(archivo, linea))
   {
       if (linea.empty()) continue;
       std::stringstream ss(linea);
       std::string id, idCliente, nombre, sMonto, pais, sTiempo, estado;
       if (!std::getline(ss, id, '|') || !std::getline(ss, idCliente, '|') ||
           !std::getline(ss, nombre, '|') || !std::getline(ss, sMonto, '|') ||
           !std::getline(ss, pais, '|') || !std::getline(ss, sTiempo, '|'))
           continue;
       std::getline(ss, estado, '|'); // Compatibilidad con archivos antiguos de seis campos.
       try
       {
           double monto = std::stod(sMonto);
           int tiempo = std::stoi(sTiempo);
           if (ingresarTransaccion(id, idCliente, nombre, monto, pais, tiempo))
           {
               // El estado se restaurará después de procesar la cola.
               // Los archivos de seis campos conservan el estado por defecto.
           }
       }
       catch (const std::exception&)
       {
           // Una línea inválida no impide cargar el resto del archivo.
           continue;
       }
   }
   archivo.close();
   procesarCola();
   return true;
}
