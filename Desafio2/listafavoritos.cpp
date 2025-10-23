#include "ListaFavoritos.h"
#include "Usuario.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include <iostream>
#include <chrono>
#include <random>

ListaFavoritos::ListaFavoritos(Usuario* usuario)
    : totalCanciones(0), capacidad(100), usuario(usuario), listaSeguida(nullptr) {
    canciones = new Cancion*[capacidad];
}

ListaFavoritos::~ListaFavoritos() {
    delete[] canciones;
}

void ListaFavoritos::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    if (nuevaCapacidad > 10000) nuevaCapacidad = 10000;

    Cancion** nuevasCanciones = new Cancion*[nuevaCapacidad];

    for (int i = 0; i < totalCanciones; i++) {
        nuevasCanciones[i] = canciones[i];
    }

    delete[] canciones;
    canciones = nuevasCanciones;
    capacidad = nuevaCapacidad;
}

bool ListaFavoritos::agregarCancion(Cancion* cancion) {
    if (cancion == nullptr || contieneCancion(cancion->getId())) {
        return false;
    }

    if (totalCanciones >= capacidad) {
        if (capacidad >= 10000) return false;
        redimensionar();
    }

    canciones[totalCanciones++] = cancion;
    return true;
}

bool ListaFavoritos::eliminarCancion(int idCancion) {
    for (int i = 0; i < totalCanciones; i++) {
        if (canciones[i]->getId() == idCancion) {
            for (int j = i; j < totalCanciones - 1; j++) {
                canciones[j] = canciones[j + 1];
            }
            totalCanciones--;
            return true;
        }
    }
    return false;
}

bool ListaFavoritos::contieneCancion(int idCancion) const {
    for (int i = 0; i < totalCanciones; i++) {
        if (canciones[i]->getId() == idCancion) {
            return true;
        }
    }
    return false;
}

int ListaFavoritos::getTotalCancionesSeguidas() const {
    if (listaSeguida != nullptr) {
        return listaSeguida->getTotalCancionesPropias(); // Consulta EN TIEMPO REAL
    }
    return 0;
}

int ListaFavoritos::getTotalCancionesVisibles() const {
    return getTotalCancionesPropias() + getTotalCancionesSeguidas();
}

Cancion* ListaFavoritos::obtenerCancion(int index) const {
    int totalPropias = getTotalCancionesPropias();

    if (index < totalPropias) {
        return canciones[index];
    }

    if (listaSeguida != nullptr) {
        int indexSeguida = index - totalPropias;
        if (indexSeguida < listaSeguida->getTotalCancionesPropias()) {
            return listaSeguida->obtenerCancion(indexSeguida); // Acceso directo EN TIEMPO REAL
        }
    }

    return nullptr;
}

Cancion* ListaFavoritos::obtenerCancionSeguida(int index) const {
    if (listaSeguida != nullptr && index < listaSeguida->getTotalCancionesPropias()) {
        return listaSeguida->obtenerCancion(index);
    }
    return nullptr;
}

void ListaFavoritos::reproducir(bool ordenAleatorio) const {
    int totalVisibles = getTotalCancionesVisibles();

    if (totalVisibles == 0) {
        std::cout << "La lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\n Reproduciendo lista de favoritos ("
              << totalVisibles << " canciones)" << std::endl;
    std::cout << " - Propias: " << getTotalCancionesPropias() << std::endl;
    std::cout << " - Seguidas: " << getTotalCancionesSeguidas() << std::endl;

    if (ordenAleatorio) {
        std::cout << "Modo: Aleatorio" << std::endl;
    } else {
        std::cout << "Modo: Orden original" << std::endl;
    }

    for (int i = 0; i < totalVisibles; i++) {
        Cancion* cancionActual = obtenerCancion(i);
        if (cancionActual == nullptr) continue;

        std::cout << "\n=== REPRODUCIENDO ===" << std::endl;
        std::cout << "Artista: " << cancionActual->getAlbum()->getArtista()->getNombre() << std::endl;
        std::cout << "Album: " << cancionActual->getAlbum()->getNombre() << std::endl;
        std::cout << "Cancion: " << cancionActual->getNombre() << std::endl;

        if (i < getTotalCancionesPropias()) {
            std::cout << "[PROPIA] ";
        } else {
            std::cout << "[SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << "] ";
        }

        std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;
        std::cout << "=====================" << std::endl;

        std::cout << "Reproduciendo..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {}
        std::cout << "Cancion finalizada." << std::endl;

        cancionActual->incrementarReproducciones();
    }

    std::cout << "\nLista de favoritos reproducida completamente." << std::endl;
}

void ListaFavoritos::seguirLista(ListaFavoritos* otraLista) {
    if (listaSeguida != nullptr) {
        std::cout << "Ya estas siguiendo una lista. Deja de seguir primero." << std::endl;
        return;
    }

    if (otraLista == this) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        return;
    }

    if (otraLista->getUsuario() == usuario) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        return;
    }

    listaSeguida = otraLista;
    std::cout << "Ahora sigues la lista de: " << otraLista->getUsuario()->getNickname() << std::endl;
}

void ListaFavoritos::dejarDeSeguirLista() {
    if (listaSeguida == nullptr) {
        std::cout << "No estas siguiendo ninguna lista." << std::endl;
        return;
    }

    std::cout << "Dejaste de seguir la lista de: " << listaSeguida->getUsuario()->getNickname() << std::endl;
    listaSeguida = nullptr;
}

void ListaFavoritos::mostrarLista() const {
    int totalVisibles = getTotalCancionesVisibles();

    if (totalVisibles == 0) {
        std::cout << "Tu lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\nTu lista de favoritos (" << totalVisibles << " canciones):" << std::endl;
    std::cout << "   Propias: " << getTotalCancionesPropias() << std::endl;
    std::cout << "   Seguidas: " << getTotalCancionesSeguidas() << std::endl;

    for (int i = 0; i < totalCanciones; i++) {
        std::cout << "[PROPIA] " << canciones[i]->getNombre()
        << " | Artista: " << canciones[i]->getAlbum()->getArtista()->getNombre() << std::endl;
    }

    if (listaSeguida != nullptr) {
        int totalSeguida = listaSeguida->getTotalCancionesPropias();
        for (int i = 0; i < totalSeguida; i++) {
            Cancion* cancionSeguida = listaSeguida->obtenerCancion(i);
            if (cancionSeguida != nullptr) {
                std::cout << "[SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << "] "
                          << cancionSeguida->getNombre()
                          << " | Artista: " << cancionSeguida->getAlbum()->getArtista()->getNombre() << std::endl;
            }
        }
    }
}
