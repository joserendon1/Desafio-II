#include "ListaFavoritos.h"
#include "Usuario.h"
#include "Cancion.h"
#include <iostream>

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
    std::cout << "Reproduciendo lista de favoritos..." << std::endl;
}

void ListaFavoritos::seguirLista(ListaFavoritos* otraLista) {
    listaSeguida = otraLista;
}
