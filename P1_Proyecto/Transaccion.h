/***********************************************************************
 * Module:  Transaccion.h
 * Purpose: Operacion bancaria con su resultado de evaluacion logica
 ***********************************************************************/

#ifndef TRANSACCION_H
#define TRANSACCION_H

#include <string>

enum Nivel { NORMAL, INUSUAL, SOSPECHOSA, ALTO_RIESGO };

class Transaccion
{
public:
   Transaccion();
   Transaccion(std::string idTransaccion, std::string idCliente, std::string nombreCliente,
               double montoBancario, std::string pais, int tiempoTransaccion);

   std::string getId() const;
   std::string getIdCliente() const;
   std::string getNombreCliente() const;
   double getMontoBancario() const;
   void setMontoBancario(double nuevoMonto);
   std::string getPais() const;
   void setPais(std::string nuevoPais);
   int getTiempoTransaccion() const;
   void setTiempoTransaccion(int nuevoTiempo);

   bool getP() const;
   void setP(bool valor);
   bool getQ() const;
   void setQ(bool valor);
   bool getR() const;
   void setR(bool valor);
   Nivel getNivel() const;
   void setNivel(Nivel nuevoNivel);
   std::string getRegla() const;
   void setRegla(std::string nuevaRegla);
   std::string getEstadoRevision() const;
   void setEstadoRevision(std::string nuevoEstado);

   std::string horaTexto() const;

private:
   // Datos de entrada
   std::string idTransaccion;
   std::string idCliente;
   std::string nombreCliente;
   double montoBancario;
   std::string pais;
   int tiempoTransaccion;      // minutos desde las 00:00

   // Resultado de la evaluacion logica
   bool P;
   bool Q;
   bool R;
   Nivel nivel;
   std::string regla;

   // Revision del analista
   std::string estadoRevision; // "pendiente", "fraude confirmado" o "falsa alarma"
};

#endif