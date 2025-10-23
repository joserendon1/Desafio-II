#include "SistemaUdeATunes.h"
#include "ListaFavoritos.h"
#include <iostream>
#include <string>
#include <limits>

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mostrarMenuPrincipal() {
    std::cout << "\n=== UdeATunes - Sistema de Streaming ===" << std::endl;
    std::cout << "1. Iniciar sesion" << std::endl;
    std::cout << "2. Mostrar metricas del sistema" << std::endl;
    std::cout << "3. Mostrar metricas de eficiencia" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuUsuario(SistemaUdeATunes& sistema) {
    std::cout << "\n=== MENU USUARIO ===" << std::endl;
    std::cout << "Usuario: " << sistema.getUsuarioActual()->getNickname() << std::endl;
    std::cout << "Membresia: " << sistema.getUsuarioActual()->getMembresia() << std::endl;
    std::cout << "1. Reproducir musica aleatoria" << std::endl;
    std::cout << "2. Mostrar canciones disponibles" << std::endl;

    if (sistema.getUsuarioActual()->esPremium()) {
        std::cout << "3. Gestionar lista de favoritos" << std::endl;
        std::cout << "4. Seguir lista de otro usuario" << std::endl;
    }

    std::cout << "0. Cerrar sesion" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void menuGestionarFavoritos(SistemaUdeATunes& sistema) {
    int opcion;
    do {
        std::cout << "\n=== GESTIONAR FAVORITOS ===" << std::endl;
        std::cout << "1. Mostrar mi lista de favoritos" << std::endl;
        std::cout << "2. Agregar cancion a favoritos" << std::endl;
        std::cout << "3. Eliminar cancion de favoritos" << std::endl;
        std::cout << "4. Reproducir lista de favoritos" << std::endl;
        std::cout << "0. Volver" << std::endl;  // Cambié a 0
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        limpiarBuffer();

        switch (opcion) {
        case 1:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr) {
                sistema.getUsuarioActual()->getListaFavoritos()->mostrarLista();
            } else {
                std::cout << "No tienes lista de favoritos." << std::endl;
            }
            break;
        case 2:
            sistema.mostrarCancionesDisponibles();
            std::cout << "Ingrese el ID de la cancion a agregar: ";
            int idCancion;
            std::cin >> idCancion;
            limpiarBuffer();
            sistema.agregarCancionAFavoritos(idCancion);
            break;
        case 3:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr) {
                sistema.getUsuarioActual()->getListaFavoritos()->mostrarLista();
                std::cout << "Ingrese el ID de la cancion a eliminar: ";
                int idEliminar;
                std::cin >> idEliminar;
                limpiarBuffer();
                if (sistema.getUsuarioActual()->getListaFavoritos()->eliminarCancion(idEliminar)) {
                    std::cout << "Cancion eliminada de favoritos." << std::endl;
                } else {
                    std::cout << "Cancion no encontrada en favoritos." << std::endl;
                }
            }
            break;
        case 4:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr) {
                std::cout << "¿Reproducir en orden aleatorio? (1=Si, 0=No): ";
                bool aleatorio;
                std::cin >> aleatorio;
                limpiarBuffer();
                sistema.getUsuarioActual()->getListaFavoritos()->reproducir(aleatorio);
            }
            break;
        case 0:
            std::cout << "Volviendo al menu principal..." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida." << std::endl;
        }
    } while (opcion != 0);
}

int main() {
    SistemaUdeATunes sistema;

    std::cout << "Cargando datos del sistema..." << std::endl;
    sistema.cargarDatos();

    int opcion;
    bool sesionActiva = false;
    bool programaActivo = true;

    do {
        if (!sesionActiva) {
            mostrarMenuPrincipal();
            std::cin >> opcion;
            limpiarBuffer();

            switch (opcion) {
            case 1:
                if (sistema.login()) {
                    sesionActiva = true;
                }
                break;
            case 2:
                sistema.mostrarMetricas();
                break;
            case 3:
                sistema.mostrarMetricasEficiencia();
                break;
            case 4:
                programaActivo = false;
                std::cout << "¡Hasta pronto!" << std::endl;
                break;
            default:
                std::cout << "Opcion no valida." << std::endl;
            }
        } else {
            mostrarMenuUsuario(sistema);
            std::cin >> opcion;
            limpiarBuffer();

            switch (opcion) {
            case 1:
                sistema.reproducirAleatorio();
                break;
            case 2:
                sistema.mostrarCancionesDisponibles();
                break;
            case 3:
                if (sistema.getUsuarioActual()->esPremium()) {
                    menuGestionarFavoritos(sistema);
                } else {
                    std::cout << "Opcion no valida." << std::endl;
                }
                break;
            case 4:
                if (sistema.getUsuarioActual()->esPremium()) {
                    std::string nickname;
                    std::cout << "Ingrese el nickname del usuario a seguir: ";
                    std::getline(std::cin, nickname);
                    if (sistema.seguirListaUsuario(nickname)) {
                        std::cout << "Operacion completada. Presione Enter para continuar...";
                        std::cin.get();
                    }
                } else {
                    std::cout << "Opcion no valida." << std::endl;
                }
                break;
            case 0:
                sistema.setUsuarioActual(nullptr);
                sesionActiva = false;
                std::cout << "Sesion cerrada." << std::endl;
                break;
            default:
                std::cout << "Opcion no valida." << std::endl;
            }
        }
    } while (programaActivo);

    return 0;
}
