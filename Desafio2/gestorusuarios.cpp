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
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        ContenedorUsuario* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    incrementarIteraciones(iteracionesLocales);

    inicio = nullptr;
    final = nullptr;
    totalUsuarios = 0;
}

bool GestorUsuarios::agregarUsuario(Usuario* usuario) {
    //resetIteraciones();

    if (usuario == nullptr) {
        incrementarIteraciones();
        return false;
    }

    if (buscarUsuario(usuario->getNickname()) != nullptr) {
        incrementarIteraciones(2);
        return false;
    }

    ContenedorUsuario* nuevoContenedor = new ContenedorUsuario(usuario);
    incrementarIteraciones();

    if (inicio == nullptr) {
        inicio = nuevoContenedor;
        final = nuevoContenedor;
        incrementarIteraciones();
    } else {
        final->siguiente = nuevoContenedor;
        final = nuevoContenedor;
        incrementarIteraciones(2);
    }

    totalUsuarios++;
    incrementarIteraciones();
    return true;
}

Usuario* GestorUsuarios::buscarUsuario(const std::string& nickname) const {
    //resetIteraciones();

    ContenedorUsuario* actual = inicio;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        if (actual->contenido->getNickname() == nickname) {
            incrementarIteraciones(iteracionesLocales);
            return actual->contenido;
        }
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return nullptr;
}

Usuario** GestorUsuarios::getUsuariosArray() const {
    //resetIteraciones();

    if (totalUsuarios == 0) {
        incrementarIteraciones();
        return nullptr;
    }

    Usuario** array = new Usuario*[totalUsuarios];
    ContenedorUsuario* actual = inicio;
    int index = 0;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales + 1);
    return array;
}

void GestorUsuarios::cargarUsuarios() {
    //resetIteraciones();

    std::ifstream archivo("datos/usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de usuarios." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::string linea;
    int iteracionesLocales = 0;
    int usuariosCargados = 0;

    while (std::getline(archivo, linea)) {
        iteracionesLocales++;
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string nickname, membresia;

        std::getline(ss, nickname, '|');
        std::getline(ss, membresia, '|');

        nickname.erase(0, nickname.find_first_not_of(" \t\n\r\f\v"));
        nickname.erase(nickname.find_last_not_of(" \t\n\r\f\v") + 1);
        membresia.erase(0, membresia.find_first_not_of(" \t\n\r\f\v"));
        membresia.erase(membresia.find_last_not_of(" \t\n\r\f\v") + 1);

        Usuario* nuevoUsuario = new Usuario(nickname, membresia);
        if (agregarUsuario(nuevoUsuario)) {
            usuariosCargados++;
            iteracionesLocales += getIteraciones();
        } else {
            delete nuevoUsuario;
        }
        resetIteraciones();
    }

    archivo.close();
    incrementarIteraciones(iteracionesLocales + 1);
}

unsigned long GestorUsuarios::calcularMemoriaUsuarios() const {
    unsigned long memoria = 0;
    ContenedorUsuario* actual = inicio;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        memoria += sizeof(ContenedorUsuario);
        memoria += sizeof(Usuario);
        memoria += actual->contenido->getNickname().capacity();
        memoria += actual->contenido->getMembresia().capacity();
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return memoria;
}
