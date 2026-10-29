/***********************************************************************
 * Module:  Archivo.cpp
 * Purpose: Implementación de la persistencia basada en el formato con delimitador '|'
 ***********************************************************************/

#include "Archivo.h"
#include <fstream>
#include <sstream>

Archivo::Archivo(std::string rutaTransacciones, std::string rutaClientes)
   : rutaTransacciones(rutaTransacciones), rutaClientes(rutaClientes)
{
}

bool Archivo::guardar(SistemaFraude& sistema)
{
   std::ofstream archivoClientes(rutaClientes);
   if (!archivoClientes.is_open())
   {
      return false;
   }
   NodoCliente* c = sistema.getClientes().inicio();
   while (c != nullptr)
   {
      Cliente* cli = c->getDato();
      archivoClientes << cli->getIdCliente() << "|" << cli->getNombreCliente() << "|"
                      << cli->getPaisHabitual() << "|" << cli->getLimiteDiario() << "\n";
      c = c->getSiguiente();
   }

   std::ofstream archivoTx(rutaTransacciones);
   if (!archivoTx.is_open())
   {
      return false;
   }
   Nodo* actual = sistema.getProcesadas().inicio();
   while (actual != nullptr)
   {
      Transaccion* t = actual->getDato();
      archivoTx << t->getIdT() << "|" << t->getIdCliente() << "|" << t->getMontoBancario() << "|"
                << t->getPaisTransaccion() << "|" << static_cast<long long>(t->getFechaHora()) << "|"
                << t->getEstadoRevision() << "\n";
      actual = actual->getSiguiente();
   }
   actual = sistema.getPendientes().inicio();
   while (actual != nullptr)
   {
      Transaccion* t = actual->getDato();
      archivoTx << t->getIdT() << "|" << t->getIdCliente() << "|" << t->getMontoBancario() << "|"
                << t->getPaisTransaccion() << "|" << static_cast<long long>(t->getFechaHora()) << "|"
                << t->getEstadoRevision() << "\n";
      actual = actual->getSiguiente();
   }
   return archivoTx.good() && archivoClientes.good();
}

bool Archivo::cargar(SistemaFraude& sistema)
{
   // 1) Clientes primero: las transacciones dependen de ellos.
   std::ifstream archivoClientes(rutaClientes);
   if (archivoClientes.is_open())
   {
      std::string linea;
      while (std::getline(archivoClientes, linea))
      {
         std::stringstream ss(linea);
         std::string id, nombre, pais, sLimite;
         if (!std::getline(ss, id, '|') || !std::getline(ss, nombre, '|') ||
             !std::getline(ss, pais, '|') || !std::getline(ss, sLimite, '|'))
         {
            continue;
         }
         try
         {
            sistema.registrarCliente(id, nombre, pais, std::stod(sLimite));
         }
         catch (std::exception&)
         {
            continue;
         }
      }
   }

   // 2) Transacciones.
   std::ifstream archivoTx(rutaTransacciones);
   if (!archivoTx.is_open())
   {
      return false;
   }
   std::string linea;
   while (std::getline(archivoTx, linea))
   {
      std::stringstream ss(linea);
      std::string id, idCliente, sMonto, pais, sFecha, estado;
      if (!std::getline(ss, id, '|') || !std::getline(ss, idCliente, '|') ||
          !std::getline(ss, sMonto, '|') || !std::getline(ss, pais, '|') ||
          !std::getline(ss, sFecha, '|'))
      {
         continue;
      }
      if (!std::getline(ss, estado, '|') || estado.empty())
      {
         estado = ESTADO_PENDIENTE;
      }
      try
      {
         sistema.cargarTransaccion(id, idCliente, std::stod(sMonto), pais,
                                   static_cast<std::time_t>(std::stoll(sFecha)), estado);
      }
      catch (std::exception&)
      {
         continue;
      }
   }

   // 3) Evalua las reglas y clasifica todo.
   sistema.procesarCola();
   return true;
}