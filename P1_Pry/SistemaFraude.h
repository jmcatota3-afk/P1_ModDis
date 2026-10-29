#ifndef SISTEMAFRAUDE_H
#define SISTEMAFRAUDE_H

#include <string>
#include <ctime>
#include "Transaccion.h"
#include "Cliente.h"
#include "ListaCliente.h"
#include "ListaSimple.h"
#include "Cola.h"
#include "Pila.h"

class SistemaFraude
{
public:
   SistemaFraude();
   ~SistemaFraude();

   // ---- Clientes ----
   bool registrarCliente(std::string idCliente, std::string nombre,
                         std::string paisHabitual, double limiteDiario);
   Cliente* buscarCliente(std::string idCliente);
   ListaCliente& getClientes();

   // ---- CRUD de transacciones ----
   bool ingresarTransaccion(std::string idCliente, double monto,
                            std::string pais, std::string& idGenerado);
   bool cargarTransaccion(std::string id, std::string idCliente, double monto,
                          std::string pais, std::time_t fechaHora,
                          std::string estadoRevision);
   Transaccion* buscarTransaccion(std::string idTransaccion);
   bool modificarTransaccion(std::string idTransaccion, double monto, std::string pais);
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

   // ---- Alertas (pila) ----
   Transaccion* verSiguienteAlerta();
   bool resolverSiguienteAlerta(bool esFraude);
   int cantidadAlertas();

   // ---- Resumen final ----
   int totalProcesadas();
   int totalPorNivel(std::string nivel);
   int totalAlertas();                            // |B U C|
   double porcentajeAlertas();

   // ---- Acceso para listar ----
   ListaSimple& getProcesadas();
   Cola& getPendientes();

private:
   bool evaluarP(Transaccion* t);
   bool evaluarQ(Transaccion* t);
   bool evaluarR(Transaccion* t);
   std::string inferir(bool P, bool Q, bool R);
   std::string describirRegla(bool P, bool Q, bool R);
   void evaluar(Transaccion* t);

   void clasificar(Transaccion* t);
   void vaciarClasificacion();
   bool existeId(std::string idTransaccion);

   ListaCliente clientes;
   ListaSimple procesadas;
   Cola pendientes;
   Pila alertas;
   ListaSimple conjuntoA;
   ListaSimple conjuntoB;
   ListaSimple conjuntoC;

   long ventanaSeg;            // 5 min = 300 s
   int maxTx;                  // R: MAS de 3 operaciones
};

#endif