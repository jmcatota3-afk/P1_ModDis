#include "SistemaFraude.h"
#include "Menu.h"

int main()
{
   SistemaFraude sistema;
   sistema.cargarArchivo();
   Menu::menuPrincipal(sistema);
   sistema.guardarArchivo();
   return 0;
}