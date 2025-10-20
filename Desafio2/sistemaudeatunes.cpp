#include "SistemaUdeATunes.h"
#include "Usuario.h"
#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include "MensajePublicitario.h"
#include <iostream>
#include <string>

SistemaUdeATunes::SistemaUdeATunes()
    : usuarios(nullptr), artistas(nullptr), albumes(nullptr),
    canciones(nullptr), mensajes(nullptr), usuarioActual(nullptr),
    totalUsuarios(0), totalArtistas(0), totalAlbumes(0),
    totalCanciones(0), totalMensajes(0), capacidad(10) {

    usuarios = new Usuario*[capacidad];
    artistas = new Artista*[capacidad];
    albumes = new Album*[capacidad];
    canciones = new Cancion*[capacidad];
    mensajes = new MensajePublicitario*[capacidad];
}

SistemaUdeATunes::~SistemaUdeATunes() {
    for (int i = 0; i < totalUsuarios; i++) delete usuarios[i];
    for (int i = 0; i < totalArtistas; i++) delete artistas[i];
    for (int i = 0; i < totalAlbumes; i++) delete albumes[i];
    for (int i = 0; i < totalCanciones; i++) delete canciones[i];
    for (int i = 0; i < totalMensajes; i++) delete mensajes[i];

    delete[] usuarios;
    delete[] artistas;
    delete[] albumes;
    delete[] canciones;
    delete[] mensajes;
}

void SistemaUdeATunes::redimensionarUsuarios() {
    int nuevaCapacidad = capacidad * 2;
    Usuario** nuevoArray = new Usuario*[nuevaCapacidad];

    for (int i = 0; i < totalUsuarios; i++) {
        nuevoArray[i] = usuarios[i];
    }

    delete[] usuarios;
    usuarios = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarArtistas() {
    int nuevaCapacidad = capacidad * 2;
    Artista** nuevoArray = new Artista*[nuevaCapacidad];

    for (int i = 0; i < totalArtistas; i++) {
        nuevoArray[i] = artistas[i];
    }

    delete[] artistas;
    artistas = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarAlbumes() {
    int nuevaCapacidad = capacidad * 2;
    Album** nuevoArray = new Album*[nuevaCapacidad];

    for (int i = 0; i < totalAlbumes; i++) {
        nuevoArray[i] = albumes[i];
    }

    delete[] albumes;
    albumes = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarCanciones() {
    int nuevaCapacidad = capacidad * 2;
    Cancion** nuevoArray = new Cancion*[nuevaCapacidad];

    for (int i = 0; i < totalCanciones; i++) {
        nuevoArray[i] = canciones[i];
    }

    delete[] canciones;
    canciones = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarMensajes() {
    int nuevaCapacidad = capacidad * 2;
    MensajePublicitario** nuevoArray = new MensajePublicitario*[nuevaCapacidad];

    for (int i = 0; i < totalMensajes; i++) {
        nuevoArray[i] = mensajes[i];
    }

    delete[] mensajes;
    mensajes = nuevoArray;
    capacidad = nuevaCapacidad;
}

bool SistemaUdeATunes::login() {
    std::string nickname;
    std::cout << "Ingrese su nickname: ";
    std::cin >> nickname;

    Usuario* usuario = buscarUsuario(nickname);
    if (usuario != nullptr) {
        usuarioActual = usuario;
        std::cout << "¡Bienvenido " << nickname << "!" << std::endl;
        return true;
    } else {
        std::cout << "Usuario no encontrado." << std::endl;
        return false;
    }
}

void SistemaUdeATunes::reproducirAleatorio() {
    if (totalCanciones == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        return;
    }

    std::cout << "Reproducción aleatoria - Funcionalidad en desarrollo" << std::endl;
}

void SistemaUdeATunes::mostrarMetricas() const {
    std::cout << "=== METRICAS DEL SISTEMA ===" << std::endl;
    std::cout << "Usuarios registrados: " << totalUsuarios << std::endl;
    std::cout << "Artistas en plataforma: " << totalArtistas << std::endl;
    std::cout << "Álbumes disponibles: " << totalAlbumes << std::endl;
    std::cout << "Canciones en catálogo: " << totalCanciones << std::endl;
    std::cout << "Mensajes publicitarios: " << totalMensajes << std::endl;
}

Usuario* SistemaUdeATunes::buscarUsuario(const std::string& nickname) const {
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i]->getNickname() == nickname) {
            return usuarios[i];
        }
    }
    return nullptr;
}

Cancion* SistemaUdeATunes::buscarCancion(int id) const {
    for (int i = 0; i < totalCanciones; i++) {
        if (canciones[i]->getId() == id) {
            return canciones[i];
        }
    }
    return nullptr;
}

Artista* SistemaUdeATunes::buscarArtista(int id) const {
    for (int i = 0; i < totalArtistas; i++) {
        if (artistas[i]->getId() == id) {
            return artistas[i];
        }
    }
    return nullptr;
}

bool SistemaUdeATunes::agregarUsuario(Usuario* usuario) {
    if (usuario == nullptr || buscarUsuario(usuario->getNickname()) != nullptr) {
        return false;
    }

    if (totalUsuarios >= capacidad) {
        redimensionarUsuarios();
    }

    usuarios[totalUsuarios++] = usuario;
    return true;
}

bool SistemaUdeATunes::agregarArtista(Artista* artista) {
    if (artista == nullptr) return false;

    if (totalArtistas >= capacidad) {
        redimensionarArtistas();
    }

    artistas[totalArtistas++] = artista;
    return true;
}

bool SistemaUdeATunes::agregarAlbum(Album* album) {
    if (album == nullptr) return false;

    if (totalAlbumes >= capacidad) {
        redimensionarAlbumes();
    }

    albumes[totalAlbumes++] = album;
    return true;
}

bool SistemaUdeATunes::agregarCancion(Cancion* cancion) {
    if (cancion == nullptr) return false;

    if (totalCanciones >= capacidad) {
        redimensionarCanciones();
    }

    canciones[totalCanciones++] = cancion;
    return true;
}

bool SistemaUdeATunes::agregarMensaje(MensajePublicitario* mensaje) {
    if (mensaje == nullptr) return false;

    if (totalMensajes >= capacidad) {
        redimensionarMensajes();
    }

    mensajes[totalMensajes++] = mensaje;
    return true;
}

void SistemaUdeATunes::cargarDatos() {
    std::cout << "Carga de datos - Funcionalidad en desarrollo" << std::endl;
}

void SistemaUdeATunes::guardarDatos() {
    std::cout << "Guardado de datos - Funcionalidad en desarrollo" << std::endl;
}
