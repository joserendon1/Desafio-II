#include "GestorCatalogo.h"
#include <iostream>
#include <fstream>
#include <sstream>

GestorCatalogo::GestorCatalogo()
    : inicioArtistas(nullptr), inicioAlbumes(nullptr), inicioCanciones(nullptr),
    totalArtistas(0), totalAlbumes(0), totalCanciones(0), iteraciones(0) {}

GestorCatalogo::~GestorCatalogo() {
    limpiarCatalogo();
}

void GestorCatalogo::limpiarArtistas() {
    ContenedorArtista* actual = inicioArtistas;
    while (actual != nullptr) {
        ContenedorArtista* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    inicioArtistas = nullptr;
    totalArtistas = 0;
}

void GestorCatalogo::limpiarAlbumes() {
    ContenedorAlbum* actual = inicioAlbumes;
    while (actual != nullptr) {
        ContenedorAlbum* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    inicioAlbumes = nullptr;
    totalAlbumes = 0;
}

void GestorCatalogo::limpiarCanciones() {
    ContenedorCancion* actual = inicioCanciones;
    while (actual != nullptr) {
        ContenedorCancion* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    inicioCanciones = nullptr;
    totalCanciones = 0;
}

void GestorCatalogo::limpiarCatalogo() {
    limpiarCanciones();
    limpiarAlbumes();
    limpiarArtistas();
}

bool GestorCatalogo::agregarArtista(Artista* artista) {
    incrementarIteraciones();

    if (artista == nullptr || buscarArtista(artista->id) != nullptr) {
        return false;
    }

    ContenedorArtista* nuevoContenedor = new ContenedorArtista(artista);

    if (inicioArtistas == nullptr) {
        inicioArtistas = nuevoContenedor;
    } else {
        ContenedorArtista* actual = inicioArtistas;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
    }

    totalArtistas++;
    return true;
}

bool GestorCatalogo::agregarAlbum(Album* album) {
    incrementarIteraciones();

    if (album == nullptr || buscarAlbum(album->id) != nullptr) {
        return false;
    }

    ContenedorAlbum* nuevoContenedor = new ContenedorAlbum(album);

    if (inicioAlbumes == nullptr) {
        inicioAlbumes = nuevoContenedor;
    } else {
        ContenedorAlbum* actual = inicioAlbumes;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
    }

    totalAlbumes++;
    return true;
}

bool GestorCatalogo::agregarCancion(Cancion* cancion) {
    incrementarIteraciones();

    if (cancion == nullptr || buscarCancion(cancion->getId()) != nullptr) {
        return false;
    }

    ContenedorCancion* nuevoContenedor = new ContenedorCancion(cancion);

    if (inicioCanciones == nullptr) {
        inicioCanciones = nuevoContenedor;
    } else {
        ContenedorCancion* actual = inicioCanciones;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
    }

    totalCanciones++;
    return true;
}

Artista* GestorCatalogo::buscarArtista(int id) const {
    incrementarIteraciones();

    ContenedorArtista* actual = inicioArtistas;
    while (actual != nullptr) {
        incrementarIteraciones();
        if (actual->contenido->id == id) {
            return actual->contenido;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Album* GestorCatalogo::buscarAlbum(int id) const {
    incrementarIteraciones();

    ContenedorAlbum* actual = inicioAlbumes;
    while (actual != nullptr) {
        incrementarIteraciones();
        if (actual->contenido->id == id) {
            return actual->contenido;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Cancion* GestorCatalogo::buscarCancion(int id) const {
    incrementarIteraciones();

    ContenedorCancion* actual = inicioCanciones;
    while (actual != nullptr) {
        incrementarIteraciones();
        if (actual->contenido->getId() == id) {
            return actual->contenido;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Artista** GestorCatalogo::getArtistasArray() const {
    if (totalArtistas == 0) return nullptr;

    Artista** array = new Artista*[totalArtistas];
    ContenedorArtista* actual = inicioArtistas;
    int index = 0;

    while (actual != nullptr) {
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    return array;
}

Album** GestorCatalogo::getAlbumesArray() const {
    if (totalAlbumes == 0) return nullptr;

    Album** array = new Album*[totalAlbumes];
    ContenedorAlbum* actual = inicioAlbumes;
    int index = 0;

    while (actual != nullptr) {
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    return array;
}

Cancion** GestorCatalogo::getCancionesArray() const {
    if (totalCanciones == 0) return nullptr;

    Cancion** array = new Cancion*[totalCanciones];
    ContenedorCancion* actual = inicioCanciones;
    int index = 0;

    while (actual != nullptr) {
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    return array;
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
        } else {
            std::cout << "Artista no encontrado para álbum ID: " << id << std::endl;
        }
    }

    archivo.close();
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

        int artistaId = id / 10000;
        int albumId = (id % 10000) / 100;
        int albumIdCompleto = artistaId * 100 + albumId;

        Album* album = buscarAlbum(albumIdCompleto);

        if (album != nullptr && album->artista->id == artistaId) {
            Cancion* nuevaCancion = new Cancion(id, nombre, album);
            nuevaCancion->setDuracion(std::stof(duracionStr));
            nuevaCancion->setRuta128(ruta128);
            nuevaCancion->setRuta320(ruta320);

            int reproducciones = std::stoi(reproduccionesStr);
            for (int i = 0; i < reproducciones; i++) {
                nuevaCancion->incrementarReproducciones();
            }

            agregarCancion(nuevaCancion);
        } else {
            std::cout << "Álbum no encontrado para canción ID: " << id << std::endl;
        }
    }

    archivo.close();
}

void GestorCatalogo::guardarCanciones() const {
    std::ofstream archivo("datos/canciones.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de canciones para guardar." << std::endl;
        return;
    }

    ContenedorCancion* actual = inicioCanciones;
    while (actual != nullptr) {
        archivo << actual->contenido->getId() << "|"
                << actual->contenido->getNombre() << "|"
                << actual->contenido->getDuracion() << "|"
                << actual->contenido->obtenerRuta(false) << "|"
                << actual->contenido->obtenerRuta(true) << "|"
                << actual->contenido->getReproducciones() << std::endl;
        actual = actual->siguiente;
    }

    archivo.close();
}
