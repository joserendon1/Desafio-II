#include "menus.h"

void limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pausarPantalla() {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.get();
}

void mostrarEncabezado(const std::string& titulo) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "           " << titulo << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void mostrarMenuPrincipal() {
    mostrarEncabezado("UdeATunes - Sistema de Streaming");
    std::cout << "1. Iniciar sesion" << std::endl;
    std::cout << "2. Metricas del sistema" << std::endl;
    std::cout << "3. Salir del sistema" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void mostrarMenuUsuario(SistemaUdeATunes& sistema) {
    mostrarEncabezado("MENU PRINCIPAL - UdeATunes");
    std::cout << "Usuario: " << sistema.getUsuarioActual()->getNickname() << std::endl;
    std::cout << "Membresia: " << sistema.getUsuarioActual()->getMembresia() << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    std::cout << "1. Reproduccion aleatoria" << std::endl;
    std::cout << "2. Ver catalogo de canciones" << std::endl;

    if (sistema.getUsuarioActual()->esPremium()) {
        std::cout << "3. Mis favoritos" << std::endl;
    }

    std::cout << "0. Cerrar sesion" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Seleccione una opcion: ";
}

void menuFavoritos(SistemaUdeATunes& sistema) {
    int opcion;
    do {
        mostrarEncabezado("GESTION DE FAVORITOS");
        std::cout << "1. Ver mi lista de favoritos" << std::endl;
        std::cout << "2. Agregar cancion a favoritos" << std::endl;
        std::cout << "3. Eliminar cancion de favoritos" << std::endl;
        std::cout << "4. Reproducir lista de favoritos" << std::endl;
        std::cout << "5. Seguir lista de otro usuario" << std::endl;
        std::cout << "6. Dejar de seguir lista actual" << std::endl;
        std::cout << "0. Volver al menu principal" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
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

        case 5: {
            std::string nickname;
            std::cout << "Ingrese el nickname del usuario a seguir: ";
            std::getline(std::cin, nickname);
            if (sistema.seguirListaUsuario(nickname)) {
                std::cout << "Operacion completada exitosamente." << std::endl;
            }
            break;
        }

        case 6:
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
            pausarPantalla();
        }
    } while (opcion != 0);
}
