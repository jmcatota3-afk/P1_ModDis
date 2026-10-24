#include "Menu.h"

void Menu::menuPrincipal(SistemaFraude& sistema)
{
   // TODO : implement
}

void Menu::menuCRUD(SistemaFraude& sistema)
{
   // TODO : implement
}

void Menu::menuConjuntos(SistemaFraude& sistema)
{
   // TODO : implement
}
// VALIDACIONES 
bool Menu::idValido(const std::string& texto)
{
   // TODO : implement
   return false;
}

bool Menu::montoValido(const std::string& texto)
{
   // TODO : implement
   return false;
}

bool Menu::soloLetras(const std::string& texto)
{
   // TODO : implement
   return false;
}

bool Menu::horaValida(const std::string& texto)
{
   // TODO : implement
   return false;
}

int Menu::horaMinutos(const std::string& texto)
{
   // TODO : implement
   return 0;
}

int Menu::leerOpcion(int min, int max)
{
   // TODO : implement
   return min;
}

std::string Menu::nombreNivel(Nivel nivel)
{
   // TODO : implement
   return "";
}