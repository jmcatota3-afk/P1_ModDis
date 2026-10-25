/***********************************************************************
 * Module:  Menu.cpp
 * Purpose: Implementación de menús interactivos y validaciones robustas
 ***********************************************************************/

#include "Menu.h"
#include "Archivo.h"
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>

void Menu::menuPrincipal(SistemaFraude& sistema)
{
    int opcion;
    Archivo archivo("transacciones.txt");
    do
    {
        std::cout << "\n==========================================================================\n";
        std::cout << "         SISTEMA DE DETECCION LOGICA DE FRAUDES - BANCO FINLOGIC          \n";
        std::cout << "==========================================================================\n";
        std::cout << "1. Gestionar Transacciones (CRUD)\n";
        std::cout << "2. Procesar Cola de Transacciones Pendientes\n";
        std::cout << "3. Gestion de Conjuntos y Operaciones (A, B, C)\n";
        std::cout << "4. Gestion de Alertas (Pila)\n";
        std::cout << "5. Guardar / Cargar Datos desde Archivo\n";
        std::cout << "6. Ver Resumen y Estadisticas Finales\n";
        std::cout << "7. Salir\n";
        std::cout << "==========================================================================\n";
        std::cout << "Ingrese una opcion: ";
        
        opcion = leerOpcion(1, 7);

        switch (opcion)
        {
            case 1:
                menuCRUD(sistema);
                break;
            case 2:
            {
                int procesadas = sistema.procesarCola();
                std::cout << "\n[OK] Se han procesado y evaluado " << procesadas << " transacciones pendientes.\n";
                break;
            }
            case 3:
                menuConjuntos(sistema);
                break;
            case 4:
            {
                int opcionAlerta;
                do
                {
                    std::cout << "\n========== GESTION DE ALERTAS (PILA) ==========\n";
                    std::cout << "Alertas pendientes en Pila: " << sistema.cantidadAlertas() << "\n";
                    std::cout << "1. Ver siguiente alerta en la cima\n";
                    std::cout << "2. Resolver / Atender siguiente alerta\n";
                    std::cout << "3. Volver\n";
                    std::cout << "Seleccione una opcion: ";
                    opcionAlerta = leerOpcion(1, 3);

                    if (opcionAlerta == 1)
                    {
                        Transaccion* t = sistema.verSiguienteAlerta();
                        if (t != nullptr)
                        {
                            std::cout << "\n[ALERTA EN CIMA]\n"
                                      << "ID: " << t->getId() << " | Cliente: " << t->getNombreCliente() 
                                      << " | Monto: $" << t->getMontoBancario() << " | Nivel: " << t->getNivel() << "\n";
                        }
                        else
                        {
                            std::cout << "\nNo hay alertas pendientes en la pila.\n";
                        }
                    }
                    else if (opcionAlerta == 2)
                    {
                        if (sistema.cantidadAlertas() > 0)
                        {
                            sistema.resolverSiguienteAlerta(true);
                            std::cout << "\n[OK] Alerta atendida y retirada de la pila exitosamente.\n";
                        }
                        else
                        {
                            std::cout << "\nNo hay alertas para resolver.\n";
                        }
                    }
                } while (opcionAlerta != 3);
                break;
            }
            case 5:
            {
                int opArchivo;
                std::cout << "\n1. Guardar transacciones en archivo\n2. Cargar transacciones desde archivo\nSeleccione opcion: ";
                opArchivo = leerOpcion(1, 2);
                if (opArchivo == 1)
                {
                    if (archivo.guardar(sistema))
                    {
                        std::cout << "\n[OK] Datos guardados exitosamente en 'transacciones.txt'.\n";
                    }
                    else
                    {
                        std::cout << "\n[Error] No se pudo abrir o guardar el archivo.\n";
                    }
                }
                else
                {
                    if (archivo.cargar(sistema))
                    {
                        std::cout << "\n[OK] Datos cargados y evaluados exitosamente.\n";
                    }
                    else
                    {
                        std::cout << "\n[Error] No se pudo abrir o cargar el archivo.\n";
                    }
                }
                break;
            }
            case 6:
            {
                std::cout << "\n================== RESUMEN FINANCIERO Y DE RIESGO ==================\n";
                std::cout << "Total de transacciones procesadas: " << sistema.totalProcesadas() << "\n";
                std::cout << " - Transacciones Normales (Conjunto A): " << sistema.totalPorNivel(NIVEL_NORMAL) << "\n";
                std::cout << " - Transacciones Sospechosas (Conjunto B): " << sistema.totalPorNivel(NIVEL_SOSPECHOSA) << "\n";
                std::cout << " - Transacciones de Alto Riesgo (Conjunto C): " << sistema.totalPorNivel(NIVEL_ALTO_RIESGO) << "\n";
                std::cout << "Total de alertas generadas (|B U C|): " << sistema.totalAlertas() << "\n";
                std::cout << "Porcentaje de alertas: " << sistema.porcentajeAlertas() << "%\n";
                std::cout << "====================================================================\n";
                break;
            }
            case 7:
                std::cout << "\nSaliendo del sistema. ¡Hasta pronto!\n";
                break;
        }
    } while (opcion != 7);
}

void Menu::menuCRUD(SistemaFraude& sistema)
{
    int opcion;
    do
    {
        std::cout << "\n ========== MENU DE OPERACIONES DE TRANSACCIONES =============\n";
        std::cout << "==========================================================================\n";
        std::cout << "1. INGRESAR\n";
        std::cout << "2. MOSTRAR (Procesadas)\n";
        std::cout << "3. BUSCAR\n";
        std::cout << "4. ELIMINAR\n";
        std::cout << "5. MODIFICAR\n";
        std::cout << "6. VOLVER\n";
        std::cout << "==========================================================================\n";
        std::cout << "Ingrese una opcion: ";
        
        opcion = leerOpcion(1, 6);

        switch(opcion)
        {
            case 1:
            {
                std::string id, idCli, nombre, pais, horaStr, montoStr;
                
                std::cout << "Ingrese ID de transaccion: ";
                std::cin >> id;
                std::cin.ignore(32767, '\n');

                std::cout << "Ingrese ID de cliente: ";
                std::cin >> idCli;
                std::cin.ignore(32767, '\n');

                std::cout << "Ingrese nombre del cliente: ";
                std::getline(std::cin, nombre);

                std::cout << "Ingrese monto bancario: ";
                std::cin >> montoStr;
                while (!montoValido(montoStr))
                {
                    std::cout << "Monto invalido. Reingrese monto: ";
                    std::cin >> montoStr;
                }
                double monto = std::stod(montoStr);

                std::cout << "Ingrese pais: ";
                std::cin >> pais;

                std::cout << "Ingrese tiempo (Formato HH:MM, ej. 14:30): ";
                std::cin >> horaStr;
                while (!horaValida(horaStr))
                {
                    std::cout << "Hora invalida. Ingrese en formato HH:MM: ";
                    std::cin >> horaStr;
                }
                int tiempoMin = horaMinutos(horaStr);

                if (sistema.ingresarTransaccion(id, idCli, nombre, monto, pais, tiempoMin))
                {
                    std::cout << "\n[OK] Transaccion ingresada a la cola con exito.\n";
                }
                else
                {
                    std::cout << "\n[Error] El ID de transaccion ya existe en el sistema.\n";
                }
                break;
            }
            case 2:
            {
                std::cout << "\n--- LISTADO DE TRANSACCIONES PROCESADAS ---\n";
                Nodo* actual = sistema.getProcesadas().inicio();
                if (actual == nullptr)
                {
                    std::cout << "No hay transacciones procesadas aun.\n";
                }
                else
                {
                    while (actual != nullptr)
                    {
                        Transaccion* t = actual->getDato();
                        if (t != nullptr)
                        {
                            std::cout << "ID: " << t->getId() 
                                      << " | Cliente: " << t->getNombreCliente() 
                                      << " | Monto: $" << t->getMontoBancario() 
                                      << " | Pais: " << t->getPais() 
                                      << " | Hora: " << t->horaTexto()
                                      << " | Nivel: " << t->getNivel() << "\n";
                        }
                        actual = actual->getSiguiente();
                    }
                }
                break;
            }
            case 3:
            {
                std::string id;
                std::cout << "Ingrese ID a buscar: ";
                std::cin >> id;
                Transaccion* t = sistema.buscarTransaccion(id);
                if (t != nullptr)
                {
                    std::cout << "\n[Transaccion Encontrada]\n"
                              << " - ID: " << t->getId() << "\n"
                              << " - ID Cliente: " << t->getIdCliente() << "\n"
                              << " - Nombre: " << t->getNombreCliente() << "\n"
                              << " - Monto: $" << t->getMontoBancario() << "\n"
                              << " - Pais: " << t->getPais() << "\n"
                              << " - Hora: " << t->horaTexto() << "\n"
                              << " - Nivel de Riesgo: " << t->getNivel() << "\n"
                              << " - Detalle Regla: " << t->getRegla() << "\n";
                }
                else
                {
                    std::cout << "\nTransaccion no encontrada en procesadas.\n";
                }
                break;
            }
            case 4:
            {
                std::string id;
                std::cout << "Ingrese ID a eliminar: ";
                std::cin >> id;
                if (sistema.eliminarTransaccion(id))
                {
                    std::cout << "\n[OK] Transaccion eliminada exitosamente.\n";
                }
                else
                {
                    std::cout << "\n[Error] No se pudo encontrar o eliminar la transaccion.\n";
                }
                break;
            }
            case 5:
            {
                std::string id, pais, horaStr, montoStr;
                std::cout << "Ingrese ID de la transaccion a modificar: ";
                std::cin >> id;
                
                Transaccion* t = sistema.buscarTransaccion(id);
                if (t == nullptr)
                {
                    std::cout << "\nTransaccion no encontrada.\n";
                    break;
                }

                std::cout << "Nuevo monto: ";
                std::cin >> montoStr;
                while (!montoValido(montoStr))
                {
                    std::cout << "Monto invalido. Reingrese monto: ";
                    std::cin >> montoStr;
                }
                double monto = std::stod(montoStr);

                std::cout << "Nuevo pais: ";
                std::cin >> pais;

                std::cout << "Nuevo tiempo (HH:MM): ";
                std::cin >> horaStr;
                while (!horaValida(horaStr))
                {
                    std::cout << "Hora invalida. Reingrese (HH:MM): ";
                    std::cin >> horaStr;
                }
                int tiempoMin = horaMinutos(horaStr);

                if (sistema.modificarTransaccion(id, monto, pais, tiempoMin))
                {
                    std::cout << "\n[OK] Transaccion modificada y reclasificada con exito.\n";
                }
                else
                {
                    std::cout << "\n[Error] No se pudo modificar la transaccion.\n";
                }
                break;
            }
            case 6:
                break;
        }
    } while(opcion != 6);
}

void Menu::menuConjuntos(SistemaFraude& sistema)
{
    int opcion;
    do
    {
        std::cout << "\n ========== MENU DE CONJUNTOS Y OPERACIONES LOGICAS ===========\n";
        std::cout << "1. Ver Conjunto A (Transacciones Normales)\n";
        std::cout << "2. Ver Conjunto B (Transacciones Sospechosas)\n";
        std::cout << "3. Ver Conjunto C (Transacciones de Alto Riesgo)\n";
        std::cout << "4. Interseccion A n B (Inusuales pero validas)\n";
        std::cout << "5. Union B u C (Requieren revision de seguridad)\n";
        std::cout << "6. Diferencia A - C (Transacciones seguras y verificadas)\n";
        std::cout << "7. Volver\n";
        std::cout << "Seleccione una opcion: ";
        opcion = leerOpcion(1, 7);

        if (opcion >= 1 && opcion <= 3)
        {
            ListaSimple* listaObj = nullptr;
            std::string nombreConjunto = "";
            if (opcion == 1) { listaObj = &sistema.getConjuntoA(); nombreConjunto = "A (Normales)"; }
            else if (opcion == 2) { listaObj = &sistema.getConjuntoB(); nombreConjunto = "B (Sospechosas)"; }
            else { listaObj = &sistema.getConjuntoC(); nombreConjunto = "C (Alto Riesgo)"; }

            std::cout << "\n--- CONJUNTO " << nombreConjunto << " ---\n";
            Nodo* actual = listaObj->inicio();
            if (actual == nullptr)
            {
                std::cout << "El conjunto esta vacio.\n";
            }
            else
            {
                while (actual != nullptr)
                {
                    Transaccion* t = actual->getDato();
                    if (t != nullptr)
                    {
                        std::cout << "ID: " << t->getId() << " | Cliente: " << t->getNombreCliente() 
                                  << " | Monto: $" << t->getMontoBancario() << " | Pais: " << t->getPais() << "\n";
                    }
                    actual = actual->getSiguiente();
                }
            }
        }
        else if (opcion >= 4 && opcion <= 6)
        {
            ListaSimple resultado;
            std::string descOp = "";
            if (opcion == 4)
            {
                sistema.interseccionAB(resultado);
                descOp = "Interseccion A n B (Comportamiento inusual pero validas)";
            }
            else if (opcion == 5)
            {
                sistema.unionBC(resultado);
                descOp = "Union B u C (Operaciones que requieren revision por seguridad)";
            }
            else
            {
                sistema.diferenciaAC(resultado);
                descOp = "Diferencia A - C (Transacciones seguras y verificadas)";
            }

            std::cout << "\n--- RESULTADO DE OPERACION: " << descOp << " ---\n";
            Nodo* actual = resultado.inicio();
            if (actual == nullptr)
            {
                std::cout << "El resultado de la operacion esta vacio.\n";
            }
            else
            {
                while (actual != nullptr)
                {
                    Transaccion* t = actual->getDato();
                    if (t != nullptr)
                    {
                        std::cout << "ID: " << t->getId() << " | Cliente: " << t->getNombreCliente() 
                                  << " | Monto: $" << t->getMontoBancario() << " | Nivel: " << t->getNivel() << "\n";
                    }
                    actual = actual->getSiguiente();
                }
            }
        }
    } while (opcion != 7);
}

// VALIDACIONES 

bool Menu::idValido(const std::string& texto)
{
    return !texto.empty();
}

bool Menu::montoValido(const std::string& texto)
{
    if (texto.empty()) return false;
    try
    {
        size_t idx;
        double val = std::stod(texto, &idx);
        return idx == texto.length() && val >= 0;
    }
    catch (...)
    {
        return false;
    }
}

bool Menu::soloLetras(const std::string& texto)
{
    if (texto.empty()) return false;
    for (char c : texto)
    {
        if (!std::isalpha(static_cast<unsigned char>(c)) && c != ' ')
        {
            return false;
        }
    }
    return true;
}

bool Menu::horaValida(const std::string& texto)
{
    if (texto.length() != 5 || texto[2] != ':') return false;
    try
    {
        int h = std::stoi(texto.substr(0, 2));
        int m = std::stoi(texto.substr(3, 2));
        return (h >= 0 && h < 24 && m >= 0 && m < 60);
    }
    catch (...)
    {
        return false;
    }
}

int Menu::horaMinutos(const std::string& texto)
{
    if (!horaValida(texto)) return 0;
    int h = std::stoi(texto.substr(0, 2));
    int m = std::stoi(texto.substr(3, 2));
    return h * 60 + m;
}

int Menu::leerOpcion(int min, int max)
{
    int opcion;
    while (true)
    {
        std::cin >> opcion;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            std::cout << "Entrada invalida. Ingrese un numero entero: ";
        }
        else
        {
            std::cin.ignore(32767, '\n');
            if (opcion >= min && opcion <= max)
            {
                return opcion;
            }
            else
            {
                std::cout << "Opcion fuera de rango (" << min << " - " << max << "). Intente de nuevo: ";
            }
        }
    }
}
