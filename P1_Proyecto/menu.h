/***********************************************************************
 * Module:  Menu.h
 * Purpose: Menus interactivos y validacion de los datos que el usuario
 *          escribe por consola
 ***********************************************************************/

#ifndef MENU_H
#define MENU_H

#include <string>
#include "SistemaFraude.h"

class Menu
{
public:
   // ---- Menus ----
   static void menuPrincipal(SistemaFraude& sistema);
   static void menuCRUD(SistemaFraude& sistema);
   static void menuConjuntos(SistemaFraude& sistema);

   // ---- Validaciones ----
   static bool idValido(const std::string& texto);
   static bool montoValido(const std::string& texto);
   static bool soloLetras(const std::string& texto);
   static bool horaValida(const std::string& texto);
   static int horaMinutos(const std::string& texto);
   static int leerOpcion(int min, int max);

   // ---- Apoyo para mostrar ----
   static std::string nombreNivel(Nivel nivel);
};

#endif