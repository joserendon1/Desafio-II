#include "GestorUsuarios.h"
#include <iostream>
#include <fstream>
#include <sstream>

GestorUsuarios::GestorUsuarios()
    : usuarios(nullptr), totalUsuarios(0), capacidad(10), iteraciones(0) {
    usuarios = new Usuario*[capacidad];
}

GestorUsuarios::~GestorUsuarios() {
    for (int i = 0; i < totalUsuarios; i++) {
        delete usuarios[i];
    }
    delete[] usuarios;
}

void GestorUsuarios::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    Usuario** nuevoArray = new Usuario*[nuevaCapacidad];

    for (int i = 0; i < totalUsuarios; i++) {
        nuevoArray[i] = usuarios[i];
    }

    delete[] usuarios;
    usuarios = nuevoArray;
    capacidad = nuevaCapacidad;

    std::cout << "🔧 GestorUsuarios redimensionado: " << capacidad/2 << " → " << capacidad << std::endl;
}

bool GestorUsuarios::agregarUsuario(Usuario* usuario) {
    incrementarIteraciones();

    if (usuario == nullptr || buscarUsuario(usuario->getNickname()) != nullptr) {
        return false;
    }

    if (totalUsuarios >= capacidad) {
        redimensionar();
    }

    usuarios[totalUsuarios++] = usuario;
    return true;
}

Usuario* GestorUsuarios::buscarUsuario(const std::string& nickname) const {
    incrementarIteraciones(); // Llamada al método

    for (int i = 0; i < totalUsuarios; i++) {
        incrementarIteraciones(); // Cada iteración
        if (usuarios[i]->getNickname() == nickname) {
            return usuarios[i];
        }
    }
    return nullptr;
}

bool GestorUsuarios::autenticarUsuario(const std::string& nickname, Usuario*& usuarioActual) const {
    incrementarIteraciones();
    Usuario* usuario = buscarUsuario(nickname);

    if (usuario != nullptr) {
        usuarioActual = usuario;
        return true;
    }
    return false;
}

bool GestorUsuarios::existeUsuario(const std::string& nickname) const {
    return buscarUsuario(nickname) != nullptr;
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
        std::string nickname, membresia, ciudad, pais, fecha;

        std::getline(ss, nickname, '|');
        std::getline(ss, membresia, '|');
        std::getline(ss, ciudad, '|');
        std::getline(ss, pais, '|');
        std::getline(ss, fecha, '|');

        Usuario* nuevoUsuario = new Usuario(nickname, membresia);
        nuevoUsuario->setCiudad(ciudad);
        nuevoUsuario->setPais(pais);
        nuevoUsuario->setFechaInscripcion(fecha);

        agregarUsuario(nuevoUsuario);
    }

    archivo.close();
    std::cout << "Usuarios cargados: " << totalUsuarios << std::endl;
}

void GestorUsuarios::guardarUsuarios() const {
    std::ofstream archivo("datos/usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de usuarios para guardar." << std::endl;
        return;
    }

    for (int i = 0; i < totalUsuarios; i++) {
        archivo << usuarios[i]->getNickname() << "|"
                << usuarios[i]->getMembresia() << "|"
                << usuarios[i]->getCiudad() << "|"
                << usuarios[i]->getPais() << "|"
                << usuarios[i]->getFechaInscripcion() << std::endl;
    }

    archivo.close();
    std::cout << "Usuarios guardados: " << totalUsuarios << std::endl;
}

void GestorUsuarios::limpiarUsuarios() {
    for (int i = 0; i < totalUsuarios; i++) {
        delete usuarios[i];
    }
    totalUsuarios = 0;
}
