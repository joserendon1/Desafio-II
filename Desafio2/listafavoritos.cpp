#include "ListaFavoritos.h"
#include "Usuario.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include <iostream>
#include <chrono>


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

void ListaFavoritos::reproducir(bool ordenAleatorio) const {
    int totalVisibles = getTotalCancionesVisibles();

    if (totalVisibles == 0) {
        std::cout << "La lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\n Reproduciendo lista de favoritos ("
              << totalVisibles << " canciones)" << std::endl;

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

        if (i < totalCanciones) {
            std::cout << "[PROPIA] ";
        } else {
            std::cout << "[SEGUIDA] ";
        }

        std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;
        std::cout << "=====================" << std::endl;

        std::cout << "Reproduciendo..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
        }
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

int ListaFavoritos::getTotalCancionesVisibles() const {
    int total = totalCanciones;

    if (listaSeguida != nullptr) {
        total += listaSeguida->getTotalCanciones();
    }

    return total;
}

Cancion* ListaFavoritos::obtenerCancion(int index) const {
    if (index < totalCanciones) {
        return canciones[index];
    }

    if (listaSeguida != nullptr) {
        int indexSeguida = index - totalCanciones;
        if (indexSeguida < listaSeguida->getTotalCanciones()) {
            return listaSeguida->getCanciones()[indexSeguida];
        }
    }

    return nullptr;
}

void ListaFavoritos::mostrarLista() const {
    int totalVisibles = getTotalCancionesVisibles();

    if (totalVisibles == 0) {
        std::cout << "Tu lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\nTu lista de favoritos (" << totalVisibles << " canciones):" << std::endl;

    for (int i = 0; i < totalCanciones; i++) {
        std::cout << "[PROPIA] " << canciones[i]->getNombre()
        << " | Artista: " << canciones[i]->getAlbum()->getArtista()->getNombre() << std::endl;
    }

    if (listaSeguida != nullptr) {
        Cancion** cancionesSeguida = listaSeguida->getCanciones();
        int totalSeguida = listaSeguida->getTotalCanciones();

        for (int i = 0; i < totalSeguida; i++) {
            std::cout << "[SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << "] "
                      << cancionesSeguida[i]->getNombre()
                      << " | Artista: " << cancionesSeguida[i]->getAlbum()->getArtista()->getNombre() << std::endl;
        }
    }
}
