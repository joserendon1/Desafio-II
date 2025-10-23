#include "GestorCatalogo.h"
#include <iostream>
#include <fstream>
#include <sstream>

GestorCatalogo::GestorCatalogo()
    : artistas(nullptr), albumes(nullptr), canciones(nullptr),
    totalArtistas(0), totalAlbumes(0), totalCanciones(0), capacidad(10), iteraciones(0) {

    artistas = new Artista*[capacidad];
    albumes = new Album*[capacidad];
    canciones = new Cancion*[capacidad];
}

GestorCatalogo::~GestorCatalogo() {
    limpiarCatalogo();
    delete[] artistas;
    delete[] albumes;
    delete[] canciones;
}

void GestorCatalogo::limpiarCatalogo() {
    for (int i = 0; i < totalArtistas; i++) delete artistas[i];
    for (int i = 0; i < totalAlbumes; i++) delete albumes[i];
    for (int i = 0; i < totalCanciones; i++) delete canciones[i];

    totalArtistas = 0;
    totalAlbumes = 0;
    totalCanciones = 0;
}

void GestorCatalogo::redimensionarArtistas() {
    int nuevaCapacidad = capacidad * 2;
    Artista** nuevoArray = new Artista*[nuevaCapacidad];

    for (int i = 0; i < totalArtistas; i++) {
        nuevoArray[i] = artistas[i];
    }

    delete[] artistas;
    artistas = nuevoArray;
    capacidad = nuevaCapacidad;
}

void GestorCatalogo::redimensionarAlbumes() {
    int nuevaCapacidad = capacidad * 2;
    Album** nuevoArray = new Album*[nuevaCapacidad];

    for (int i = 0; i < totalAlbumes; i++) {
        nuevoArray[i] = albumes[i];
    }

    delete[] albumes;
    albumes = nuevoArray;
    capacidad = nuevaCapacidad;
}

void GestorCatalogo::redimensionarCanciones() {
    int nuevaCapacidad = capacidad * 2;
    Cancion** nuevoArray = new Cancion*[nuevaCapacidad];

    for (int i = 0; i < totalCanciones; i++) {
        nuevoArray[i] = canciones[i];
    }

    delete[] canciones;
    canciones = nuevoArray;
    capacidad = nuevaCapacidad;
}

bool GestorCatalogo::agregarArtista(Artista* artista) {
    incrementarIteraciones();

    if (artista == nullptr || buscarArtista(artista->id) != nullptr) {
        return false;
    }

    if (totalArtistas >= capacidad) {
        redimensionarArtistas();
    }

    artistas[totalArtistas++] = artista;
    return true;
}

bool GestorCatalogo::agregarAlbum(Album* album) {
    incrementarIteraciones();

    if (album == nullptr || buscarAlbum(album->id) != nullptr) {
        return false;
    }

    if (totalAlbumes >= capacidad) {
        redimensionarAlbumes();
    }

    albumes[totalAlbumes++] = album;
    return true;
}

bool GestorCatalogo::agregarCancion(Cancion* cancion) {
    incrementarIteraciones();

    if (cancion == nullptr || buscarCancion(cancion->getId()) != nullptr) {
        return false;
    }

    if (totalCanciones >= capacidad) {
        redimensionarCanciones();
    }

    canciones[totalCanciones++] = cancion;
    return true;
}

Artista* GestorCatalogo::buscarArtista(int id) const {
    incrementarIteraciones();

    for (int i = 0; i < totalArtistas; i++) {
        incrementarIteraciones();
        if (artistas[i]->id == id) {
            return artistas[i];
        }
    }
    return nullptr;
}

Album* GestorCatalogo::buscarAlbum(int id) const {
    incrementarIteraciones();

    for (int i = 0; i < totalAlbumes; i++) {
        incrementarIteraciones();
        if (albumes[i]->id == id) {
            return albumes[i];
        }
    }
    return nullptr;
}

Cancion* GestorCatalogo::buscarCancion(int id) const {
    incrementarIteraciones();

    for (int i = 0; i < totalCanciones; i++) {
        incrementarIteraciones();
        if (canciones[i]->getId() == id) {
            return canciones[i];
        }
    }
    return nullptr;
}

void GestorCatalogo::cargarArtistas() {
    std::ifstream archivo("datos/artistas.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de artistas." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, nombre;

        std::getline(ss, idStr, '|');
        std::getline(ss, nombre, '|');

        int id = std::stoi(idStr);
        Artista* nuevoArtista = new Artista(id, nombre);

        agregarArtista(nuevoArtista);
    }

    archivo.close();
    std::cout << "Artistas cargados: " << totalArtistas << std::endl;
}

void GestorCatalogo::cargarAlbumes() {
    std::ifstream archivo("datos/albumes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de álbumes." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, nombre, portada;

        std::getline(ss, idStr, '|');
        std::getline(ss, nombre, '|');
        std::getline(ss, portada, '|');

        int id = std::stoi(idStr);
        int artistaId = id / 100;
        Artista* artista = buscarArtista(artistaId);

        if (artista != nullptr) {
            Album* nuevoAlbum = new Album(id, nombre, artista);
            nuevoAlbum->portada = portada;

            agregarAlbum(nuevoAlbum);
        }
    }

    archivo.close();
    std::cout << "Álbumes cargados: " << totalAlbumes << std::endl;
}

void GestorCatalogo::cargarCanciones() {
    std::ifstream archivo("datos/canciones.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de canciones." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, nombre, duracionStr, ruta128, ruta320, reproduccionesStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, nombre, '|');
        std::getline(ss, duracionStr, '|');
        std::getline(ss, ruta128, '|');
        std::getline(ss, ruta320, '|');
        std::getline(ss, reproduccionesStr, '|');

        int id = std::stoi(idStr);
        float duracion = std::stof(duracionStr);
        int reproducciones = std::stoi(reproduccionesStr);

        int albumId = id / 100;
        Album* album = buscarAlbum(albumId);

        if (album != nullptr) {
            Cancion* nuevaCancion = new Cancion(id, nombre, album);
            nuevaCancion->setDuracion(duracion);
            nuevaCancion->setRuta128(ruta128);
            nuevaCancion->setRuta320(ruta320);

            for (int i = 0; i < reproducciones; i++) {
                nuevaCancion->incrementarReproducciones();
            }

            agregarCancion(nuevaCancion);
        }
    }

    archivo.close();
    std::cout << "Canciones cargadas: " << totalCanciones << std::endl;
}

void GestorCatalogo::guardarCanciones() const {
    std::ofstream archivo("datos/canciones.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de canciones para guardar." << std::endl;
        return;
    }

    for (int i = 0; i < totalCanciones; i++) {
        archivo << canciones[i]->getId() << "|"
                << canciones[i]->getNombre() << "|"
                << canciones[i]->getDuracion() << "|"
                << canciones[i]->obtenerRuta(false) << "|"
                << canciones[i]->obtenerRuta(true) << "|"
                << canciones[i]->getReproducciones() << std::endl;
    }

    archivo.close();
}
