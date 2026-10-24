#include "Menu.h"
#include <iostream>
#include <string>
#include <cctype>

void Menu::menuPrincipal(SistemaFraude& sistema)
{
   // TODO : implement
}

void Menu::menuCRUD(SistemaFraude& sistema)
{
   int opcion;
   do
   {
      printf("\n ========== MENU DE OPERACIONES DE TRANSACCIONES =============\n");
      printf("==========================================================================\n");
      printf("1. INGRESAR\n");
      printf("2. MOSTRAR\n");
      printf("3. BUSCAR\n");
      printf("4. ELIMINAR\n");
      printf("5. MODIFICAR\n");
      printf("6. VOLVER\n");
      printf("==========================================================================\n");
      printf("Ingrese una opcion:\n");
  //  pendiente la funcion 
      switch(opcion){
          
      }
    

    }while(opcion!=6);
      /* code */
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

