/***********************************************************************
 * Module:  SistemaFraude.h
 * Purpose: Coordina todo: CRUD, evaluacion logica, conjuntos, alertas,
 *          resumen y archivo
 * Comment: Es el UNICO dueno de las transacciones: las crea con new y
 *          las libera en el destructor. Las demas estructuras solo
 *          guardan punteros.
 ***********************************************************************/

#ifndef SISTEMAFRAUDE_H
#define SISTEMAFRAUDE_H

#include <string>
#include "Transaccion.h"
#include "ListaSimple.h"
#include "Cola.h"
#include "Pila.h"

class SistemaFraude
{
public:
   SistemaFraude();
   ~SistemaFraude();
   SistemaFraude(const SistemaFraude& otro) = delete;
   SistemaFraude& operator=(const SistemaFraude& otro) = delete;

   // ---- CRUD ----
   bool ingresarTransaccion(std::string idTransaccion, std::string idCliente,
                            std::string nombreCliente, double montoBancario,
                            std::string pais, int tiempoTransaccion);
   Transaccion* buscarTransaccion(std::string idTransaccion);
   bool modificarTransaccion(std::string idTransaccion, double montoBancario,
                             std::string pais, int tiempoTransaccion);
   bool eliminarTransaccion(std::string idTransaccion);

   // ---- Procesamiento ----
   int procesarCola();
   void reclasificarTodo();

   // ---- Conjuntos A, B y C ----
   ListaSimple& getConjuntoA();
   ListaSimple& getConjuntoB();
   ListaSimple& getConjuntoC();
   void interseccionAB(ListaSimple& resultado);   // A n B
   void unionBC(ListaSimple& resultado);          // B U C
   void diferenciaAC(ListaSimple& resultado);     // A - C

   // ---- Alertas ----
   Transaccion* verSiguienteAlerta();
   bool resolverSiguienteAlerta(bool esFraude);
   int cantidadAlertas();

   // ---- Resumen final ----
   int totalProcesadas();
   int totalPorNivel(Nivel nivel);
   int totalAlertas();                            // |B U C|
   double porcentajeAlertas();

   // ---- Acceso para listar ----
   ListaSimple& getProcesadas();
   Cola& getPendientes();

   // ---- Archivo ----
   bool guardarArchivo();
   bool cargarArchivo();

private:
   // Evaluacion logica
   bool evaluarP(Transaccion* t);
   bool evaluarQ(Transaccion* t);
   bool evaluarR(Transaccion* t);
   Nivel inferir(bool P, bool Q, bool R);
   std::string describirRegla(bool P, bool Q, bool R);
   void evaluar(Transaccion* t);
   std::string paisHabitual(std::string idCliente, Transaccion* hasta);

   // Clasificacion y apoyo
   void clasificar(Transaccion* t);
   void vaciarClasificacion();
   bool existeId(std::string idTransaccion);

   ListaSimple procesadas;     // duena de las transacciones ya evaluadas
   Cola pendientes;            // duena de las transacciones sin evaluar
   Pila alertas;
   ListaSimple conjuntoA;
   ListaSimple conjuntoB;
   ListaSimple conjuntoC;

   double limiteDiario;
   int ventanaMin;
   int maxTx;
   std::string rutaArchivo;
};

#endif