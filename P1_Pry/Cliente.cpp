#include "Cliente.h"

Cliente::Cliente(const string& idCliente, const string& nombreCliente,
                 const string& paisHabitual, double limiteDiario) : idCliente(idCliente), nombreCliente(nombreCliente),
     paisHabitual(paisHabitual), limiteDiario(limiteDiario)
{
}

string Cliente::getIdCliente() const
{
   return idCliente;
}

string Cliente::getNombreCliente() const
{
   return nombreCliente;
}

string Cliente::getPaisHabitual() const
{
   return paisHabitual;
}

double Cliente::getLimiteDiario() const
{
   return limiteDiario;
}