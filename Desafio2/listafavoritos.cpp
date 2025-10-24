#include "ListaFavoritos.h"
#include "Usuario.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include <iostream>
#include <chrono>
#include <random>

ListaFavoritos::ListaFavoritos(Usuario* usuario)
    : inicio(nullptr), totalCanciones(0), usuario(usuario), listaSeguida(nullptr) {}

ListaFavoritos::~ListaFavoritos() {
    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        ContenedorCancionFavorita* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

bool ListaFavoritos::agregarCancion(Cancion* cancion) {
    if (cancion == nullptr || contieneCancion(cancion->getId())) {
        return false;
    }

    if (totalCanciones >= 10000) {
        std::cout << "Limite alcanzado: No se pueden agregar mas de 10,000 canciones a favoritos." << std::endl;
        return false;
    }

    ContenedorCancionFavorita* nuevoContenedor = new ContenedorCancionFavorita(cancion);

    if (inicio == nullptr) {
        inicio = nuevoContenedor;
    } else {
        ContenedorCancionFavorita* actual = inicio;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
    }

    totalCanciones++;
    std::cout << "Cancion agregada a favoritos: " << cancion->getNombre() << std::endl;
    return true;
}

bool ListaFavoritos::eliminarCancion(int idCancion) {
    ContenedorCancionFavorita* actual = inicio;
    ContenedorCancionFavorita* anterior = nullptr;

    while (actual != nullptr) {
        if (actual->contenido->getId() == idCancion) {
            if (anterior == nullptr) {
                inicio = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }
            delete actual;
            totalCanciones--;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

bool ListaFavoritos::contieneCancion(int idCancion) const {
    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        if (actual->contenido->getId() == idCancion) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

Cancion** ListaFavoritos::getCancionesArray() const {
    if (totalCanciones == 0) return nullptr;

    Cancion** array = new Cancion*[totalCanciones];
    ContenedorCancionFavorita* actual = inicio;
    int index = 0;

    while (actual != nullptr) {
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    return array;
}

int ListaFavoritos::getTotalCancionesSeguidas() const {
    if (listaSeguida != nullptr) {
        return listaSeguida->getTotalCanciones();
    }
    return 0;
}

int ListaFavoritos::getTotalCancionesVisibles() const {
    return getTotalCanciones() + getTotalCancionesSeguidas();
}

Cancion* ListaFavoritos::obtenerCancion(int index) const {
    if (index < 0 || index >= totalCanciones) {
        return nullptr;
    }

    ContenedorCancionFavorita* actual = inicio;
    int contador = 0;

    while (actual != nullptr && contador < index) {
        actual = actual->siguiente;
        contador++;
    }

    return (actual != nullptr) ? actual->contenido : nullptr;
}

void ListaFavoritos::reproducir(bool ordenAleatorio) const {
    int totalVisibles = getTotalCancionesVisibles();

    if (totalVisibles == 0) {
        std::cout << "La lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "       REPRODUCIENDO LISTA DE FAVORITOS" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalVisibles << std::endl;
    std::cout << " - Propias: " << getTotalCanciones() << std::endl;
    std::cout << " - Seguidas: " << getTotalCancionesSeguidas() << std::endl;

    if (ordenAleatorio) {
        std::cout << "Modo: Aleatorio" << std::endl;
    } else {
        std::cout << "Modo: Orden original" << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

    ContenedorCancionFavorita* actual = inicio;
    int cancionIndex = 0;

    while (actual != nullptr) {
        Cancion* cancionActual = actual->contenido;

        std::cout << "\nREPRODUCIENDO:" << std::endl;
        std::cout << "Artista: " << cancionActual->getAlbum()->artista->nombre << std::endl;
        std::cout << "Album: " << cancionActual->getAlbum()->nombre << std::endl;
        std::cout << "Cancion: " << cancionActual->getNombre() << std::endl;
        std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;
        std::cout << "Tipo: PROPIA" << std::endl;
        std::cout << "------------------------------------------" << std::endl;

        std::cout << "Reproduciendo..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {}
        std::cout << "Cancion finalizada." << std::endl;

        cancionActual->incrementarReproducciones();
        actual = actual->siguiente;
        cancionIndex++;
    }

    if (listaSeguida != nullptr) {
        Cancion** cancionesSeguidas = listaSeguida->getCancionesArray();
        if (cancionesSeguidas != nullptr) {
            for (int i = 0; i < listaSeguida->getTotalCanciones(); i++) {
                Cancion* cancionActual = cancionesSeguidas[i];

                std::cout << "\nREPRODUCIENDO:" << std::endl;
                std::cout << "Artista: " << cancionActual->getAlbum()->artista->nombre << std::endl;
                std::cout << "Album: " << cancionActual->getAlbum()->nombre << std::endl;
                std::cout << "Cancion: " << cancionActual->getNombre() << std::endl;
                std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;
                std::cout << "Tipo: SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << std::endl;
                std::cout << "------------------------------------------" << std::endl;

                std::cout << "Reproduciendo..." << std::endl;
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {}
                std::cout << "Cancion finalizada." << std::endl;

                cancionActual->incrementarReproducciones();
            }
            delete[] cancionesSeguidas;
        }
    }

    std::cout << "\nLista de favoritos reproducida completamente." << std::endl;
}

void ListaFavoritos::seguirLista(ListaFavoritos* otraLista) {
    if (listaSeguida != nullptr) {
        std::cout << "Ya estás siguiendo una lista. Deja de seguir primero." << std::endl;
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
        std::cout << "No estás siguiendo ninguna lista." << std::endl;
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

    std::cout << "\n==========================================" << std::endl;
    std::cout << "          LISTA DE FAVORITOS" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalVisibles << std::endl;
    std::cout << " - Propias: " << getTotalCanciones() << std::endl;
    std::cout << " - Seguidas: " << getTotalCancionesSeguidas() << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        std::cout << "[PROPIA] " << actual->contenido->getNombre()
        << " | Artista: " << actual->contenido->getAlbum()->artista->nombre << std::endl;
        actual = actual->siguiente;
    }

    if (listaSeguida != nullptr) {
        Cancion** cancionesSeguidas = listaSeguida->getCancionesArray();
        if (cancionesSeguidas != nullptr) {
            for (int i = 0; i < listaSeguida->getTotalCanciones(); i++) {
                std::cout << "[SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << "] "
                          << cancionesSeguidas[i]->getNombre()
                          << " | Artista: " << cancionesSeguidas[i]->getAlbum()->artista->nombre << std::endl;
            }
            delete[] cancionesSeguidas;
        }
    }
    std::cout << "==========================================" << std::endl;
}
