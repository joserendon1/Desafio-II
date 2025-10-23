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
    if (totalCanciones == 0) {
        std::cout << "La lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\n Reproduciendo lista de favoritos ("
              << totalCanciones << " canciones)" << std::endl;

    if (ordenAleatorio) {
        std::cout << "Modo: Aleatorio" << std::endl;
        // Aquí podrías implementar reproducción aleatoria
    } else {
        std::cout << "Modo: Orden original" << std::endl;
    }

    // Reproducir cada canción
    for (int i = 0; i < totalCanciones; i++) {
        std::cout << "\n=== REPRODUCIENDO ===" << std::endl;
        std::cout << "Artista: " << canciones[i]->getAlbum()->getArtista()->getNombre() << std::endl;
        std::cout << "Album: " << canciones[i]->getAlbum()->getNombre() << std::endl;
        std::cout << "Cancion: " << canciones[i]->getNombre() << std::endl;
        std::cout << "Duracion: " << canciones[i]->getDuracion() << " minutos" << std::endl;
        std::cout << "=====================" << std::endl;

        // Simular reproducción
        std::cout << "Reproduciendo..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
            // Esperar 2 segundos
        }
        std::cout << "Cancion finalizada." << std::endl;

        canciones[i]->incrementarReproducciones();
    }

    std::cout << "\nLista de favoritos reproducida completamente." << std::endl;
}

void ListaFavoritos::seguirLista(ListaFavoritos* otraLista) {
    if (otraLista == nullptr) {
        std::cout << "La lista a seguir no existe." << std::endl;
        return;
    }

    if (listaSeguida != nullptr) {
        std::cout << "Ya estas siguiendo la lista de: " << listaSeguida->getUsuario()->getNickname() << std::endl;
        std::cout << "Debes dejar de seguir la lista actual primero." << std::endl;
        return;
    }

    if (otraLista->getUsuario() == usuario) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        return;
    }

    listaSeguida = otraLista;
    std::cout << "Ahora sigues la lista de favoritos de: " << otraLista->getUsuario()->getNickname() << std::endl;
    std::cout << "Canciones en su lista: " << otraLista->getTotalCanciones() << std::endl;
}

void ListaFavoritos::dejarDeSeguirLista() {
    if (listaSeguida == nullptr) {
        std::cout << "No estas siguiendo ninguna lista." << std::endl;
        return;
    }

    std::cout << "Has dejado de seguir la lista de: " << listaSeguida->getUsuario()->getNickname() << std::endl;
    listaSeguida = nullptr;
}

void ListaFavoritos::mostrarLista() const {
    if (totalCanciones == 0) {
        std::cout << "Tu lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\nTu lista de favoritos (" << totalCanciones << " canciones):" << std::endl;
    for (int i = 0; i < totalCanciones; i++) {
        if (canciones[i] != nullptr &&
            canciones[i]->getAlbum() != nullptr &&
            canciones[i]->getAlbum()->getArtista() != nullptr) {

            std::cout << " " << canciones[i]->getNombre()
                      << " | Artista: " << canciones[i]->getAlbum()->getArtista()->getNombre()
                      << " | Album: " << canciones[i]->getAlbum()->getNombre()
                      << " | ID: " << canciones[i]->getId() << std::endl;
        } else {
            std::cout << "🎵 Cancion con datos incompletos" << std::endl;
        }
    }
}
