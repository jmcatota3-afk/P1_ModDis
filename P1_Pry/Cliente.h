/***********************************************************************
 * Module:  Cliente.h
 * Purpose: Persona registrada en el banco.
 
 ***********************************************************************/
#ifndef CLIENTE_H
#define CLIENTE_H
#include <string>
using namespace std;
class Cliente {
    Cliente(const string& idCliente, const string& nombreCliente,
           const string& paisHabitual, double limiteDiario);
 
   string getIdCliente() const;
   string getNombreCliente() const;
   string getPaisHabitual() const;
   double getLimiteDiario() const;
 
private:
   string idCliente;
   string nombreCliente;
    string paisHabitual;
   double limiteDiario;
};
#endif
