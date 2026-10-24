#include "SistemaFraude.h"

SistemaFraude::SistemaFraude()
   : limiteDiario(5000.0), ventanaMin(5), maxTx(3)
{
}

SistemaFraude::~SistemaFraude()
{
   // TODO : implement
}

bool SistemaFraude::ingresarTransaccion(std::string idTransaccion, std::string idCliente,std::string nombreCliente, double montoBancario,std::string pais, int tiempoTransaccion)
{
   // TODO : implement
   return false;
}

Transaccion* SistemaFraude::buscarTransaccion(std::string idTransaccion)
{
   // TODO : implement
   return nullptr;
}

bool SistemaFraude::modificarTransaccion(std::string idTransaccion, double montoBancario,std::string pais, int tiempoTransaccion)
{
   // TODO : implement
   return false;
}

bool SistemaFraude::eliminarTransaccion(std::string idTransaccion)
{
   // TODO : implement
   return false;
}

int SistemaFraude::procesarCola()
{
   // TODO : implement
   return 0;
}

void SistemaFraude::reclasificarTodo()
{
   // TODO : implement
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
   // TODO : implement
}

void SistemaFraude::unionBC(ListaSimple& resultado)
{
   // TODO : implement
}

void SistemaFraude::diferenciaAC(ListaSimple& resultado)
{
   // TODO : implement
}

Transaccion* SistemaFraude::verSiguienteAlerta()
{
   return alertas.tope();
}

bool SistemaFraude::resolverSiguienteAlerta(bool esFraude)
{
   // TODO : implement
   return false;
}

int SistemaFraude::cantidadAlertas()
{
   return alertas.tamano();
}

int SistemaFraude::totalProcesadas()
{
   return procesadas.tamano();
}

int SistemaFraude::totalPorNivel(Nivel nivel)
{
   // TODO : implement
   return 0;
}

int SistemaFraude::totalAlertas()
{
   // TODO : implement
   return 0;
}

double SistemaFraude::porcentajeAlertas()
{
   // TODO : implement
   return 0.0;
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
   // TODO : implement
   return false;
}

bool SistemaFraude::evaluarQ(Transaccion* t)
{
   // TODO : implement
   return false;
}

bool SistemaFraude::evaluarR(Transaccion* t)
{
   // TODO : implement
   return false;
}

Nivel SistemaFraude::inferir(bool P, bool Q, bool R)
{
   // TODO : implement
   return NORMAL;
}

std::string SistemaFraude::describirRegla(bool P, bool Q, bool R)
{
   // TODO : implement
   return "";
}

void SistemaFraude::evaluar(Transaccion* t)
{
   // TODO : implement
}

std::string SistemaFraude::paisHabitual(std::string idCliente, Transaccion* hasta)
{
   // TODO : implement
   return "";
}

void SistemaFraude::clasificar(Transaccion* t)
{
   // TODO : implement
}

void SistemaFraude::vaciarClasificacion()
{
   // TODO : implement
}

bool SistemaFraude::existeId(std::string idTransaccion)
{
   // TODO : implement
   return false;
}