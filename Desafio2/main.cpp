#include <iostream>
#include "SistemaUdeATunes.h"
#include "usuario.h"
#include "listafavoritos.h"

void mostrarMenuPrincipal() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "       BIENVENIDO A UdeATunes     " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Ingresar a la plataforma" << std::endl;
    std::cout << "2. Mostrar metricas del sistema" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuUsuarioEstandar() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "          MENU PRINCIPAL          " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Reproduccion aleatoria" << std::endl;
    std::cout << "2. Mostrar metricas" << std::endl;
    std::cout << "3. Cerrar sesion" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuPremium() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "          MENU PREMIUM           " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Reproduccion aleatoria" << std::endl;
    std::cout << "2. Mi lista de favoritos" << std::endl;
    std::cout << "3. Mostrar metricas" << std::endl;
    std::cout << "4. Cerrar sesion" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuFavoritos() {
    std::cout << "\n==================================" << std::endl;
    std::cout << "        MIS FAVORITOS            " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Ver mi lista" << std::endl;
    std::cout << "2. Agregar cancion" << std::endl;
    std::cout << "3. Eliminar cancion" << std::endl;
    std::cout << "4. Reproducir lista" << std::endl;
    std::cout << "5. Volver al menu principal" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void manejarMenuFavoritos(SistemaUdeATunes& sistema) {
    int opcionFav;
    do {
        mostrarMenuFavoritos();
        std::cin >> opcionFav;

        switch (opcionFav) {
        case 1:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr) {
                sistema.getUsuarioActual()->getListaFavoritos()->mostrarLista();
            } else {
                std::cout << "No tienes lista de favoritos." << std::endl;
            }
            break;
        case 2:
        {
            sistema.mostrarCancionesDisponibles();
            std::cout << "Ingrese el ID de la cancion a agregar: ";
            int idCancion;
            std::cin >> idCancion;
            sistema.agregarCancionAFavoritos(idCancion);
        }
        break;
        case 3:
        {
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr &&
                sistema.getUsuarioActual()->getListaFavoritos()->getTotalCanciones() > 0) {

                sistema.getUsuarioActual()->getListaFavoritos()->mostrarLista();
                std::cout << "Ingrese el ID de la cancion a eliminar: ";
                int idCancion;
                std::cin >> idCancion;

                if (sistema.getUsuarioActual()->getListaFavoritos()->eliminarCancion(idCancion)) {
                    std::cout << "Cancion eliminada de favoritos." << std::endl;
                } else {
                    std::cout << "Cancion no encontrada en favoritos." << std::endl;
                }
            } else {
                std::cout << "No hay canciones en tu lista de favoritos." << std::endl;
            }
        }
        break;
        case 4:
            if (sistema.getUsuarioActual()->getListaFavoritos() != nullptr &&
                sistema.getUsuarioActual()->getListaFavoritos()->getTotalCanciones() > 0) {

                std::cout << "¿Reproducir en orden aleatorio? (1=Si, 0=No): ";
                bool ordenAleatorio;
                std::cin >> ordenAleatorio;
                sistema.getUsuarioActual()->getListaFavoritos()->reproducir(ordenAleatorio);
            } else {
                std::cout << "No hay canciones en tu lista de favoritos." << std::endl;
            }
            break;
        case 5:
            std::cout << "Volviendo al menu principal..." << std::endl;
            break;
        default:
            std::cout << "Opción no valida." << std::endl;
        }
    } while (opcionFav != 5);
}

int main() {
    SistemaUdeATunes sistema;
    int opcion;

    std::cout << "=== SISTEMA UdeATunes INICIADO ===" << std::endl;

    do {
        if (sistema.getUsuarioActual() == nullptr) {

            mostrarMenuPrincipal();
            std::cin >> opcion;

            switch (opcion) {
            case 1:
                sistema.login();
                break;
            case 2:
                sistema.mostrarMetricasEficiencia();
                break;
            case 3:
                std::cout << "Gracias por usar UdeATunes Hasta pronto." << std::endl;
                break;
            default:
                std::cout << "Opcion no valida. Intente de nuevo." << std::endl;
                break;
            }
        } else {

            if (sistema.getUsuarioActual()->esPremium()) {

                mostrarMenuPremium();
                std::cin >> opcion;

                switch (opcion) {
                case 1:
                    sistema.reproducirAleatorio();
                    break;
                case 2:
                    manejarMenuFavoritos(sistema);
                    break;
                case 3:
                    sistema.mostrarMetricasEficiencia();
                    break;
                case 4:
                    sistema.setUsuarioActual(nullptr);
                    std::cout << "Sesion cerrada exitosamente." << std::endl;
                    break;
                default:
                    std::cout << "Opcion no valida. Intente de nuevo." << std::endl;
                    break;
                }
            } else {

                mostrarMenuUsuarioEstandar();
                std::cin >> opcion;

                switch (opcion) {
                case 1:
                    sistema.reproducirAleatorio();
                    break;
                case 2:
                    sistema.mostrarMetricasEficiencia();
                    break;
                case 3:
                    sistema.setUsuarioActual(nullptr);
                    std::cout << "Sesion cerrada exitosamente." << std::endl;
                    break;
                default:
                    std::cout << "Opcion no valida. Intente de nuevo." << std::endl;
                    break;
                }
            }
        }
    } while (opcion != 3 || sistema.getUsuarioActual() != nullptr);

    return 0;
}
