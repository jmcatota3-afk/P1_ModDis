#include "Transaccion.h"
#include <sstream>
#include <iomanip>
//Contador de todoas las transacciones 
int Transaccion::idTransaccion = 0;


// localtime() devuelve un puntero a un buffer compartido; se copia el
// resultado para poder comparar dos fechas sin que una pise a la otra.

static tm aFechaLocal(time_t t)
{
   return *localtime(&t);
}

//Constructores
//Trabajamos con este constructor para sistemafraude 
Transaccion::Transaccion(string idCliente, double montoBancario, string paisTransaccion): idT(generarId()), idCliente(idCliente), montoBancario(montoBancario),paisTransaccion(paisTransaccion), fechaHora(time(nullptr)),
     P(false), Q(false), R(false), nivel(NIVEL_SIN_EVALUAR), reglaUtilizada(""), estadoRevision(ESTADO_PENDIENTE)
{
}
//Constructor para cargar desde el archivo 
Transaccion::Transaccion(string iD, string idCliente, double montoBancario,string paisTransaccion, time_t fechaHora)
   : idT(iD), idCliente(idCliente), montoBancario(montoBancario),paisTransaccion(paisTransaccion), fechaHora(fechaHora),P(false), Q(false), R(false), nivel(NIVEL_SIN_EVALUAR),
     reglaUtilizada(""), estadoRevision(ESTADO_PENDIENTE)
{
   sincronizarContador(iD);
}
string Transaccion::getIdT()
{
   return idT;
}

time_t Transaccion::getFechaHora()
{
   return fechaHora;
}

string Transaccion::getFecha()
{
   tm f = aFechaLocal(fechaHora);
   ostringstream salida;
   salida << setfill('0')
          << setw(4) << (f.tm_year + 1900) << "-"
          << setw(2) << (f.tm_mon + 1) << "-"
          << setw(2) << f.tm_mday;
   return salida.str();
}
 string Transaccion::getHora()
{
   tm f = aFechaLocal(fechaHora);
   ostringstream salida;
   salida << setfill('0')
          << setw(2) << f.tm_hour << ":"
          << setw(2) << f.tm_min << ":"
          << setw(2) << f.tm_sec;
   return salida.str();
}
// datos a ingresar 
string Transaccion::getIdCliente()
{
   return idCliente;
}

void Transaccion::setIdCliente(string newIdCliente)
{
   idCliente = newIdCliente;
}

double Transaccion::getMontoBancario()
{
   return montoBancario;
}

void Transaccion::setMontoBancario(double newMontoBancario)
{
   montoBancario = newMontoBancario;
}

string Transaccion::getPaisTransaccion()
{
   return paisTransaccion;
}

void Transaccion::setPaisTransaccion(string newPaisTransaccion)
{
   paisTransaccion = newPaisTransaccion;
}

string Transaccion::generarId()
{
   idTransaccion++;
   string numero = to_string(idTransaccion);
   while (numero.size() < 4)
   {
      numero = "0" + numero;      // 1 -> "0001"
   }
   return numero;
}

void Transaccion::sincronizarContador(string idCargado)
{
   // Al cargar de archivo, el contador sigue desde el ID mas alto cargado
   int numero = atoi(idCargado.c_str());
   if (numero > idTransaccion)
   {
      idTransaccion = numero;
   }
}

// logica 
bool Transaccion::getP()
{
   return P;
}

void Transaccion::setP(bool valor)
{
   P = valor;
}

bool Transaccion::getQ()
{
   return Q;
}

void Transaccion::setQ(bool valor)
{
   Q = valor;
}

bool Transaccion::getR()
{
   return R;
}

void Transaccion::setR(bool valor)
{
   R = valor;
}
string Transaccion::getNivel()
{
   return nivel;
}

void Transaccion::setNivel(string nuevoNivel)
{
   nivel = nuevoNivel;
}

string Transaccion::getReglaUliziada()
{
   return reglaUtilizada;
}

void Transaccion::setRegla(string nuevaReglaUtilizada)
{
   reglaUtilizada = nuevaReglaUtilizada;
}
void Transaccion::setClasificacion(bool p, bool q, bool r,string nuevoNivel, string nuevaReglaUtilizada)
{
   P = p;
   Q = q;
   R = r;
   nivel = nuevoNivel;
   reglaUtilizada = nuevaReglaUtilizada;
}
bool Transaccion::estaEvaluada()
{
   return nivel != NIVEL_SIN_EVALUAR;
}
//REVISION ESTADO 
string Transaccion::getEstadoRevision()
{
   return estadoRevision;
}

void Transaccion::setEstadoRevision(string nuevoEstado)
{
   estadoRevision = nuevoEstado;
}
bool Transaccion::mismaFecha(Transaccion* otraTransaccion)
{
   if (otraTransaccion == nullptr)
   {
      return false;
   }
   tm a = aFechaLocal(fechaHora);
   tm b = aFechaLocal(otraTransaccion->fechaHora);
   return a.tm_year == b.tm_year && a.tm_yday == b.tm_yday;
}
long Transaccion::segundosEntre(Transaccion* otraTransaccion)
{
   if (otraTransaccion == nullptr)
   {
      return 0;
   }
   long diferencia = static_cast<long>(fechaHora - otraTransaccion->fechaHora);
   return diferencia < 0 ? -diferencia : diferencia;
}