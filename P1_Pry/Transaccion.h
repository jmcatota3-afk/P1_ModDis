/***********************************************************************
 * Module:  Transaccion.h
 * Purpose: Operacion bancaria con su resultado de evaluacion logica
 ***********************************************************************/

#ifndef TRANSACCION_H
#define TRANSACCION_H
#include <ctime>
using namespace std;

#include <string>

//Nivel de Riego 
const string NIVEL_SIN_EVALUAR = "SIN EVALUAR";
const string NIVEL_NORMAL      = "NORMAL";
const string NIVEL_INUSUAL     = "INUSUAL";
const string NIVEL_SOSPECHOSA  = "SOSPECHOSA";
const string NIVEL_ALTO_RIESGO = "ALTO RIESGO";

//Revision 
const string ESTADO_PENDIENTE = "Pendiente";
const string ESTADO_FRAUDE = "Fraude Confirmado";
const string ESTADO_FALSA_ALARMA = "Falsa Alarma";

class Transaccion
{
public: 
Transaccion(string idCliente, double montoBancario, string paisTransaccion);

Transaccion(string iD,string idCliente, double montoBancario,string paisTransaccion, std::time_t fechaHora);

   string getIdT();
   time_t getFechaHora();      // segundos desde 1970 (fecha + hora exactas)
   string getFecha();          // "AAAA-MM-DD"
   string getHora();            // HORA-MINUTO-SEGUNDOS 
   string getIdCliente();
   void setIdCliente(string newIdCliente);
   double getMontoBancario();
   void setMontoBancario(double newMontoBancario);
   string getPaisTransaccion();
   void setPaisTransaccion(string newPaisTransaccion);
   //EVALUACION LOGICA 
   bool getP();
   void setP(bool valor);
   bool getQ();
   void setQ(bool valor);
   bool getR();
   void setR(bool valor);
   string getNivel(); 
   void setNivel(string nuevoNivel);
   string getReglaUliziada();                  // regla de inferencia aplicada
   void setRegla(std::string nuevaReglaUtilizada);
   //Asignacion de P,Q,R y el NIvel 
   void setClasificacion(bool p, bool q, bool r,string nuevoNivel, string nuevaReglaUtilizada);
   bool estaEvaluada();
   //Estado de Transacciones 
   string getEstadoRevision();
   void setEstadoRevision(string nuevoEstado);
   //limite de transaccion de P Y r
   bool mismaFecha(Transaccion* otraTransaccion); 
   long segundosEntre(Transaccion* otraTransaccion); 


private:
   // Datos de entrada
   static int idTransaccion; // Contador transacciones 
   static string generarId();
   static void sincronizarContador(string idCargado);
   string idT;
   string idCliente; 
   double montoBancario;
   string paisTransaccion;
   time_t fechaHora;    // Hora Exacta 

   // Resultado de la evaluacion logica
   bool P;
   bool Q;
   bool R;
   string nivel;          
   string reglaUtilizada;

   // Revision del analista
   string estadoRevision;
};

#endif