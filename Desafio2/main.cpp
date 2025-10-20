#include <iostream>
#include "SistemaUdeATunes.h"

void mostrarMenuPrincipal() {
    std::cout << "==================================" << std::endl;
    std::cout << "       BIENVENIDO A UdeATunes     " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Ingresar a la plataforma" << std::endl;
    std::cout << "2. Salir" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

int main() {
    SistemaUdeATunes sistema;
    int opcion;

    do {
        mostrarMenuPrincipal();
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            sistema.login();
            break;
        case 2:
            std::cout << "¡Gracias por usar UdeATunes!" << std::endl;
            break;
        default:
            std::cout << "Opcion no valida. Intente de nuevo." << std::endl;
        }
    } while (opcion != 2);


    return 0;
}
