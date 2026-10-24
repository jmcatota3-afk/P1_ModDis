/***********************************************************************
 * Module:  Archivo.h
 * Purpose: Guarda y carga las transacciones en un archivo de texto
 * Comment: Formato, una transaccion por linea:
 *          id|idCliente|nombreCliente|monto|pais|tiempo|estadoRevision
 *          Solo guarda datos de entrada y la revision del analista.
 *          P, Q, R y el nivel NO se guardan: se recalculan al cargar.
 ***********************************************************************/

#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>
#include "SistemaFraude.h"

class Archivo
{
public:
   Archivo(std::string ruta);

   bool guardar(SistemaFraude& sistema);
   bool cargar(SistemaFraude& sistema);

private:
   std::string ruta;
};

#endif