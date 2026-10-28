#include "SistemaFraude.h"
#include "Menu.h"
#include "Archivo.h"

int main()
{
    SistemaFraude sistema;
    // Centraliza la persistencia en Archivo para mantener un formato único.
    Archivo archivo("transacciones.txt");
    archivo.cargar(sistema);
    Menu::menuPrincipal(sistema);
    archivo.guardar(sistema);
    return 0;
}
