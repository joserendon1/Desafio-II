#include "SistemaUdeATunes.h"
#include "menus.h"

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
                } else {
                    pausarPantalla();
                }
                break;

            case 2:
                sistema.mostrarMetricas();
                sistema.mostrarMetricasEficiencia();
                pausarPantalla();
                break;

            case 3:
                programaActivo = false;
                std::cout << "Gracias por usar UdeATunes. Hasta pronto" << std::endl;
                break;

            default:
                std::cout << "Opcion no valida. Intente nuevamente." << std::endl;
                pausarPantalla();
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
                pausarPantalla();
                break;
            case 3:
                if (sistema.getUsuarioActual()->esPremium()) {
                    menuFavoritos(sistema);
                } else {
                    std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                    pausarPantalla();
                }
                break;
            case 0:
                sistema.setUsuarioActual(nullptr);
                sesionActiva = false;
                std::cout << "Sesion cerrada correctamente." << std::endl;
                break;
            default:
                std::cout << "Opcion no valida. Intente nuevamente." << std::endl;
                pausarPantalla();
            }
        }
    } while (programaActivo);

    return 0;
}
