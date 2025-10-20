#include <iostream>
#include "SistemaUdeATunes.h"

void mostrarMenuPrincipal() {
    std::cout << "==================================" << std::endl;
    std::cout << "       BIENVENIDO A UdeATunes     " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Ingresar a la plataforma" << std::endl;
    std::cout << "2. Mostrar métricas del sistema" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuUsuario() {
    std::cout << "==================================" << std::endl;
    std::cout << "          MENÚ PRINCIPAL          " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Reproducción aleatoria" << std::endl;
    std::cout << "2. Mostrar métricas" << std::endl;
    std::cout << "3. Cerrar sesión" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

int main() {
    SistemaUdeATunes sistema;
    int opcion;

    do {
        if (sistema.getUsuarioActual() == nullptr) {
            mostrarMenuPrincipal();
            std::cin >> opcion;

            switch (opcion) {
            case 1:
                sistema.login();
                break;
            case 2:
                sistema.mostrarMetricas();
                break;
            case 3:
                std::cout << "¡Gracias por usar UdeATunes!" << std::endl;
                break;
            default:
                std::cout << "Opción no válida. Intente de nuevo." << std::endl;
            }
        } else {
            mostrarMenuUsuario();
            std::cin >> opcion;

            switch (opcion) {
            case 1:
                sistema.reproducirAleatorio();
                break;
            case 2:
                sistema.mostrarMetricas();
                break;
            case 3:
                std::cout << "Cerrando sesión..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida. Intente de nuevo." << std::endl;
            }
        }
    } while (opcion != 3);

    return 0;
}
