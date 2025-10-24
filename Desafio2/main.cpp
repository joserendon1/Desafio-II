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
    std::cout << "\n==========================================" << std::endl;
    std::cout << "      UdeATunes - Sistema de Streaming" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "1. Iniciar sesion" << std::endl;
    std::cout << "2. Metricas del sistema" << std::endl;
    std::cout << "3. Metricas de eficiencia" << std::endl;
    std::cout << "4. Salir del sistema" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuUsuario(SistemaUdeATunes& sistema) {

    std::cout << "\n==========================================" << std::endl;
    std::cout << "      MENU PRINCIPAL - UdeATunes" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Usuario: " << sistema.getUsuarioActual()->getNickname() << std::endl;
    std::cout << "Membresia: " << sistema.getUsuarioActual()->getMembresia() << std::endl;

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "1. Reproduccion aleatoria" << std::endl;
    std::cout << "2. Ver catalogo de canciones" << std::endl;

    if (sistema.getUsuarioActual()->esPremium()) {
        std::cout << "3. Mis favoritos" << std::endl;
        std::cout << "4. Seguir lista de usuario" << std::endl;
    }

    std::cout << "0. Cerrar sesion" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void menuGestionarFavoritos(SistemaUdeATunes& sistema) {
    int opcion;
    do {
        std::cout << "\n==========================================" << std::endl;
        std::cout << "      GESTION DE FAVORITOS" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "1. Ver mi lista de favoritos" << std::endl;
        std::cout << "2. Agregar cancion a favoritos" << std::endl;
        std::cout << "3. Eliminar cancion de favoritos" << std::endl;
        std::cout << "4. Reproducir lista de favoritos" << std::endl;
        std::cout << "5. Dejar de seguir lista actual" << std::endl;
        std::cout << "0. Volver al menu principal" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
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
                    std::cout << "Cancion eliminada de favoritos correctamente." << std::endl;
                } else {
                    std::cout << "Cancion no encontrada en favoritos." << std::endl;
                }
            }
            break;
        case 4:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr) {
                std::cout << "Reproducir en orden aleatorio? (1=Si, 0=No): ";
                bool aleatorio;
                std::cin >> aleatorio;
                limpiarBuffer();
                sistema.reproducirListaFavoritos(aleatorio);
            }
            break;
        case 5:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr) {
                sistema.dejarDeSeguirLista();
            }
            break;
        case 0:
            std::cout << "Volviendo al menu principal..." << std::endl;
            break;
        default:
            std::cout << "Opcion no valida. Intente nuevamente." << std::endl;
        }

        if (opcion != 0) {
            std::cout << "\nPresione Enter para continuar...";
            std::cin.get();
        }
    } while (opcion != 0);
}

int main() {

    SistemaUdeATunes sistema;
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
                sistema.incrementarIteraciones(1);
                break;

            case 3:
                sistema.mostrarMetricasEficiencia();
                sistema.incrementarIteraciones(1);
                break;
            case 4:
                programaActivo = false;
                std::cout << "Gracias por usar UdeATunes. Hasta pronto!" << std::endl;
                break;
            default:
                std::cout << "Opcion no valida. Intente nuevamente." << std::endl;
            }
        } else {
            mostrarMenuUsuario(sistema);
            std::cin >> opcion;
            limpiarBuffer();

            if (sistema.getUsuarioActual()->esPremium()) {
                switch (opcion) {
                case 1:
                    sistema.reproducirAleatorio();
                    break;
                case 2:
                    sistema.mostrarCancionesDisponibles();
                    break;
                case 3:
                    menuGestionarFavoritos(sistema);
                    break;
                case 4:
                {
                    std::string nickname;
                    std::cout << "Ingrese el nickname del usuario a seguir: ";
                    std::getline(std::cin, nickname);
                    if (sistema.seguirListaUsuario(nickname)) {
                        std::cout << "Operacion completada exitosamente." << std::endl;
                    }
                }
                break;
                case 0:
                    sistema.setUsuarioActual(nullptr);
                    sesionActiva = false;
                    std::cout << "Sesion cerrada correctamente." << std::endl;
                    break;
                default:
                    std::cout << "Opcion no valida. Intente nuevamente." << std::endl;
                }
            }
            else {
                switch (opcion) {
                case 1:
                    sistema.reproducirAleatorio();
                    break;
                case 2:
                    sistema.mostrarCancionesDisponibles();
                    break;
                case 0:
                    sistema.setUsuarioActual(nullptr);
                    sesionActiva = false;
                    std::cout << "Sesion cerrada correctamente." << std::endl;
                    break;
                default:
                    std::cout << "Opcion no valida. Intente nuevamente." << std::endl;
                }
            }
        }
    } while (programaActivo);

    return 0;
}
