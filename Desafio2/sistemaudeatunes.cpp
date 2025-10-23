#include "SistemaUdeATunes.h"
#include "Usuario.h"
#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include "MensajePublicitario.h"
#include "reproductor.h"
#include "ListaFavoritos.h"
#include "GestorUsuarios.h"
#include "GestorCatalogo.h"
#include "GestorAlmacenamiento.h"
#include <iostream>
#include <string>
//#include <fstream>
//#include <sstream>

SistemaUdeATunes::SistemaUdeATunes()
    : gestorUsuarios(new GestorUsuarios()),
    gestorCatalogo(new GestorCatalogo()),
    gestorAlmacenamiento(new GestorAlmacenamiento(gestorUsuarios, gestorCatalogo)),
    usuarioActual(nullptr), reproductor(nullptr),
    totalIteraciones(0), memoriaConsumida(0) {

}

SistemaUdeATunes::~SistemaUdeATunes() {
    gestorAlmacenamiento->guardarCambios();  // CAMBIAR: guardarCambios en lugar de guardarTodo

    if (reproductor != nullptr) {
        delete reproductor;
    }

    delete gestorAlmacenamiento;
    delete gestorCatalogo;
    delete gestorUsuarios;
}


void SistemaUdeATunes::cargarDatos() {
    gestorAlmacenamiento->cargarTodo();
}

bool SistemaUdeATunes::login() {
    std::string nickname;
    std::cout << "Ingrese su nickname: ";
    std::cin >> nickname;

    Usuario* usuario = gestorUsuarios->buscarUsuario(nickname);
    if (usuario != nullptr) {
        usuarioActual = usuario;
        std::cout << "¡Bienvenido " << nickname << "!" << std::endl;
        std::cout << "Membresía: " << usuario->getMembresia() << std::endl;
        return true;
    } else {
        std::cout << "Usuario no encontrado." << std::endl;
        return false;
    }
}

void SistemaUdeATunes::reproducirAleatorio() {
    if (gestorCatalogo->getTotalCanciones() == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        return;
    }

    if (usuarioActual == nullptr) {
        std::cout << "Debe iniciar sesion para reproducir musica." << std::endl;
        return;
    }

    Reproductor* nuevoReproductor = new Reproductor(
        gestorCatalogo->getCanciones(),
        gestorCatalogo->getTotalCanciones(),
        gestorAlmacenamiento->getMensajes(),
        gestorAlmacenamiento->getTotalMensajes(),
        usuarioActual
        );

    if (reproductor != nullptr) {
        delete reproductor;
    }

    reproductor = nuevoReproductor;
    reproductor->reproducirAleatorio();
}

void SistemaUdeATunes::incrementarIteraciones(int cantidad) const {
    totalIteraciones += cantidad;
}

void SistemaUdeATunes::mostrarMetricas() const {
    std::cout << "=== MÉTRICAS DEL SISTEMA ===" << std::endl;
    std::cout << "Usuarios registrados: " << gestorUsuarios->getTotalUsuarios() << std::endl;
    std::cout << "Artistas en plataforma: " << gestorCatalogo->getTotalArtistas() << std::endl;
    std::cout << "Álbumes disponibles: " << gestorCatalogo->getTotalAlbumes() << std::endl;
    std::cout << "Canciones en catálogo: " << gestorCatalogo->getTotalCanciones() << std::endl;
    std::cout << "Mensajes publicitarios: " << gestorAlmacenamiento->getTotalMensajes() << std::endl;
}

Usuario* SistemaUdeATunes::buscarUsuario(const std::string& nickname) const {
    return gestorUsuarios->buscarUsuario(nickname);
}

Cancion* SistemaUdeATunes::buscarCancion(int id) const {
    return gestorCatalogo->buscarCancion(id);
}

Artista* SistemaUdeATunes::buscarArtista(int id) const {
    return gestorCatalogo->buscarArtista(id);
}

Album* SistemaUdeATunes::buscarAlbum(int id) const {
    return gestorCatalogo->buscarAlbum(id);
}

void SistemaUdeATunes::mostrarCancionesDisponibles() const {
    int totalCanciones = gestorCatalogo->getTotalCanciones();
    Cancion** canciones = gestorCatalogo->getCanciones();

    std::cout << "\nCanciones disponibles (" << totalCanciones << "):" << std::endl;
    for (int i = 0; i < totalCanciones; i++) {
        std::cout << "ID: " << canciones[i]->getId()
        << " | " << canciones[i]->getNombre()
        << " - " << canciones[i]->getAlbum()->getArtista()->getNombre() << std::endl;
    }
}

bool SistemaUdeATunes::agregarCancionAFavoritos(int idCancion) {
    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden tener listas de favoritos." << std::endl;
        return false;
    }

    Cancion* cancion = gestorCatalogo->buscarCancion(idCancion);
    if (cancion == nullptr) {
        std::cout << "Cancion no encontrada." << std::endl;
        return false;
    }

    if (usuarioActual->getListaFavoritos()->agregarCancion(cancion)) {
        std::cout << "Cancion agregada a favoritos: " << cancion->getNombre() << std::endl;
        return true;
    } else {
        std::cout << "La cancion ya esta en favoritos o la lista está llena." << std::endl;
        return false;
    }
}

void SistemaUdeATunes::calcularMemoria() const {
    memoriaConsumida = 0;

    memoriaConsumida += gestorUsuarios->getTotalUsuarios() * (sizeof(Usuario) + 50);
    memoriaConsumida += gestorCatalogo->getTotalArtistas() * (sizeof(Artista) + 40);
    memoriaConsumida += gestorCatalogo->getTotalAlbumes() * (sizeof(Album) + 100);
    memoriaConsumida += gestorCatalogo->getTotalCanciones() * (sizeof(Cancion) + 80);
    memoriaConsumida += gestorAlmacenamiento->getTotalMensajes() * (sizeof(MensajePublicitario) + 60);
}

void SistemaUdeATunes::mostrarMetricasEficiencia() const {
    calcularMemoria();

    std::cout << "\n=== MÉTRICAS DE EFICIENCIA ===" << std::endl;
    std::cout << "Total de iteraciones: " << totalIteraciones << std::endl;
    std::cout << "Memoria consumida: " << memoriaConsumida << " bytes" << std::endl;
    std::cout << "≈ " << memoriaConsumida / 1024 << " KB" << std::endl;
    std::cout << "===============================" << std::endl;
}

bool SistemaUdeATunes::seguirListaUsuario(const std::string& nicknameSeguido) {
    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden seguir listas." << std::endl;
        return false;
    }

    if (nicknameSeguido == usuarioActual->getNickname()) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        return false;
    }

    incrementarIteraciones();
    Usuario* usuarioSeguido = gestorUsuarios->buscarUsuario(nicknameSeguido);

    if (usuarioSeguido == nullptr) {
        std::cout << "Usuario '" << nicknameSeguido << "' no encontrado." << std::endl;
        return false;
    }

    if (!usuarioSeguido->esPremium()) {
        std::cout << "El usuario '" << nicknameSeguido << "' no es premium y no tiene lista de favoritos." << std::endl;
        return false;
    }

    if (usuarioSeguido->getListaFavoritos() == nullptr) {
        std::cout << "El usuario '" << nicknameSeguido << "' no tiene lista de favoritos." << std::endl;
        return false;
    }

    usuarioActual->getListaFavoritos()->seguirLista(usuarioSeguido->getListaFavoritos());
    return true;
}

void SistemaUdeATunes::dejarDeSeguirLista() {
    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden seguir listas." << std::endl;
        return;
    }

    if (usuarioActual->getListaFavoritos() == nullptr) {
        std::cout << "No tienes lista de favoritos." << std::endl;
        return;
    }

    usuarioActual->getListaFavoritos()->dejarDeSeguirLista();
}
