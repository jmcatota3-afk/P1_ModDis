/***********************************************************************
 * Module:  Archivo.cpp
 * Purpose: Implementación de la persistencia basada en el formato con delimitador '|'
 ***********************************************************************/

#include "Archivo.h"
#include "Transaccion.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

Archivo::Archivo(std::string ruta) : ruta(ruta)
{
}

bool Archivo::guardar(SistemaFraude& sistema)
{
   std::ofstream archivo(ruta);
   if (!archivo.is_open())
   {
       return false;
   }

   // Obtenemos la lista de transacciones procesadas del sistema
   ListaSimple& procesadas = sistema.getProcesadas();
   Nodo* actual = procesadas.inicio();

   while (actual != nullptr)
   {
       Transaccion* t = actual->getDato();
       if (t != nullptr)
       {
           // Formato especificado: id|idCliente|nombreCliente|monto|pais|tiempo|estadoRevision
           archivo << t->getId() << "|"
                   << t->getIdCliente() << "|"
                   << t->getNombreCliente() << "|"
                   << t->getMontoBancario() << "|"
                   << t->getPais() << "|"
                   << t->getTiempoTransaccion() << "|"
                   << t->getEstadoRevision() << "\n";
       }
       actual = actual->getSiguiente();
   }

   archivo.close();
   return true;
}

bool Archivo::cargar(SistemaFraude& sistema)
{
   std::ifstream archivo(ruta);
   if (!archivo.is_open())
   {
       return false;
   }

   std::string linea;
   struct InfoRevision {
       std::string id;
       std::string estado;
   };
   std::vector<InfoRevision> pendientesRevision;

   while (std::getline(archivo, linea))
   {
       if (linea.empty()) continue;

       std::stringstream ss(linea);
       std::string idTransaccion, idCliente, nombreCliente, sMonto, pais, sTiempo, estadoRevision;

       // Extraer campos separados por barra vertical '|'
       if (std::getline(ss, idTransaccion, '|') &&
           std::getline(ss, idCliente, '|') &&
           std::getline(ss, nombreCliente, '|') &&
           std::getline(ss, sMonto, '|') &&
           std::getline(ss, pais, '|') &&
           std::getline(ss, sTiempo, '|') &&
           std::getline(ss, estadoRevision, '|'))
       {
           try
           {
               double montoBancario = std::stod(sMonto);
               int tiempoTransaccion = std::stoi(sTiempo);

               // Ingresamos la transacción al sistema (se encola)
               if (sistema.ingresarTransaccion(idTransaccion, idCliente, nombreCliente, montoBancario, pais, tiempoTransaccion))
               {
                   pendientesRevision.push_back({idTransaccion, estadoRevision});
               }
           }
           catch (...)
           {
               // Si ocurre un error de conversión en una línea, se omite y continúa
               continue;
           }
       }
   }

   archivo.close();
   
   // Procesamos la cola para que se evalúen las reglas lógicas y se clasifiquen las transacciones
   sistema.procesarCola();

   // Restauramos el estado de revisión original del analista para cada transacción cargada
   for (const auto& item : pendientesRevision)
   {
       Transaccion* t = sistema.buscarTransaccion(item.id);
       if (t != nullptr)
       {
           t->setEstadoRevision(item.estado);
       }
   }

   return true;
}