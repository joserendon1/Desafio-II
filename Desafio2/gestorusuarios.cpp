#include "GestorUsuarios.h"
#include <iostream>
#include <fstream>
#include <sstream>

GestorUsuarios::GestorUsuarios()
    : inicio(nullptr), final(nullptr), totalUsuarios(0), iteraciones(0) {}

GestorUsuarios::~GestorUsuarios() {
    limpiarUsuarios();
}

void GestorUsuarios::limpiarUsuarios() {
    ContenedorUsuario* actual = inicio;
    while (actual != nullptr) {
        ContenedorUsuario* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    inicio = nullptr;
    final = nullptr;
    totalUsuarios = 0;
}

bool GestorUsuarios::agregarUsuario(Usuario* usuario) {
    incrementarIteraciones();

    if (usuario == nullptr || buscarUsuario(usuario->getNickname()) != nullptr) {
        return false;
    }

    ContenedorUsuario* nuevoContenedor = new ContenedorUsuario(usuario);

    if (inicio == nullptr) {
        inicio = nuevoContenedor;
        final = nuevoContenedor;
    } else {
        final->siguiente = nuevoContenedor;
        final = nuevoContenedor;
    }

    totalUsuarios++;
    return true;
}

Usuario* GestorUsuarios::buscarUsuario(const std::string& nickname) const {
    incrementarIteraciones();

    ContenedorUsuario* actual = inicio;
    while (actual != nullptr) {
        incrementarIteraciones();
        if (actual->contenido->getNickname() == nickname) {
            return actual->contenido;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Usuario** GestorUsuarios::getUsuariosArray() const {
    if (totalUsuarios == 0) return nullptr;

    Usuario** array = new Usuario*[totalUsuarios];
    ContenedorUsuario* actual = inicio;
    int index = 0;

    while (actual != nullptr) {
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    return array;
}

void GestorUsuarios::cargarUsuarios() {
    std::ifstream archivo("datos/usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de usuarios." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string nickname, membresia;

        std::getline(ss, nickname, '|');
        std::getline(ss, membresia, '|');

        Usuario* nuevoUsuario = new Usuario(nickname, membresia);
        agregarUsuario(nuevoUsuario);
    }

    archivo.close();
}
