#include "Transaccion.h"

Transaccion::Transaccion()
   : P(false), Q(false), R(false), nivel(NORMAL), estadoRevision("pendiente")
{
}

Transaccion::Transaccion(std::string idTransaccion, std::string idCliente,std::string nombreCliente, double montoBancario,std::string pais, int tiempoTransaccion)
   : idTransaccion(idTransaccion), idCliente(idCliente), nombreCliente(nombreCliente),
     montoBancario(montoBancario), pais(pais), tiempoTransaccion(tiempoTransaccion),
     P(false), Q(false), R(false), nivel(NORMAL), estadoRevision("pendiente")
{
}

std::string Transaccion::getId() const { 
    return idTransaccion;
 }
std::string Transaccion::getIdCliente() const { 
    return idCliente; 
}
std::string Transaccion::getNombreCliente() const { 
    return nombreCliente;
 }
double Transaccion::getMontoBancario() const { 
    return montoBancario;
 }
void Transaccion::setMontoBancario(double nuevoMonto) { 
    montoBancario = nuevoMonto; 
}
std::string Transaccion::getPais() const { 
    return pais; 
}
void Transaccion::setPais(std::string nuevoPais) { 
    pais = nuevoPais; 
}
int Transaccion::getTiempoTransaccion() const { 
    return tiempoTransaccion;
 }
void Transaccion::setTiempoTransaccion(int nuevoTiempo) { 
    tiempoTransaccion = nuevoTiempo; 
}

bool Transaccion::getP() const { 
    return P; 
}
void Transaccion::setP(bool valor) { 
    P = valor; 
}
bool Transaccion::getQ() const { 
    return Q; 
}
void Transaccion::setQ(bool valor) { 
    Q = valor; 
}
bool Transaccion::getR() const { 
    return R; 
}
void Transaccion::setR(bool valor) { 
    R = valor;
 }
Nivel Transaccion::getNivel() const { 
    return nivel;
 }
void Transaccion::setNivel(Nivel nuevoNivel) { 
    nivel = nuevoNivel; 
}
std::string Transaccion::getRegla() const { 
    return regla; 
}
void Transaccion::setRegla(std::string nuevaRegla) { 
    regla = nuevaRegla; 
}
std::string Transaccion::getEstadoRevision() const { 
    return estadoRevision;
 }
void Transaccion::setEstadoRevision(std::string nuevoEstado) { 
    estadoRevision = nuevoEstado; 
}

std::string Transaccion::horaTexto() const
{
   int horas = (tiempoTransaccion / 60) % 24;
   int minutos = tiempoTransaccion % 60;

   std::string hh = (horas < 10 ? "0" : "") + std::to_string(horas);
   std::string mm = (minutos < 10 ? "0" : "") + std::to_string(minutos);
   return hh + ":" + mm;
}