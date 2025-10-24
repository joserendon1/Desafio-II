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
    int lineNumber = 0;
    int artistasCargados = 0;

    while (std::getline(archivo, linea)) {
        lineNumber++;
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, nombre;

        if (std::getline(ss, idStr, '|') && std::getline(ss, nombre, '|')) {

            idStr.erase(0, idStr.find_first_not_of(" \t\n\r\f\v"));
            idStr.erase(idStr.find_last_not_of(" \t\n\r\f\v") + 1);
            nombre.erase(0, nombre.find_first_not_of(" \t\n\r\f\v"));
            nombre.erase(nombre.find_last_not_of(" \t\n\r\f\v") + 1);

            try {
                int id = std::stoi(idStr);
                Artista* nuevoArtista = new Artista(id, nombre);

                if (agregarArtista(nuevoArtista)) {
                    artistasCargados++;
                } else {
                    delete nuevoArtista;
                }

            } catch (const std::exception& e) {
                std::cout << "ERROR linea " << lineNumber << ": " << e.what() << std::endl;
                continue;
            }
        } else {
            std::cout << "ERROR linea " << lineNumber << ": Formato incorrecto" << std::endl;
        }
    }

    archivo.close();
}

void GestorCatalogo::cargarAlbumes() {
    std::ifstream archivo("datos/albumes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de albumes." << std::endl;
        return;
    }

    std::string linea;
    int lineNumber = 0;
    int albumesCargados = 0;
    int albumesConErrores = 0;

    while (std::getline(archivo, linea)) {
        lineNumber++;
        if (linea.empty()) continue;

        int totalCampos = 0;
        for (char c : linea) {
            if (c == '|') totalCampos++;
        }
        totalCampos++;

        if (totalCampos < 7) {
            std::cout << "❌ ERROR linea " << lineNumber << ": Album necesita al menos 7 campos, tiene " << totalCampos << std::endl;
            albumesConErrores++;
            continue;
        }

        std::stringstream ss(linea);
        std::string campo;
        std::string idStr, nombre, portada;

        if (!std::getline(ss, idStr, '|')) {
            albumesConErrores++;
            continue;
        }

        if (!std::getline(ss, nombre, '|')) {
            albumesConErrores++;
            continue;
        }

        for (int i = 0; i < 4; i++) {
            if (!std::getline(ss, campo, '|')) {
                albumesConErrores++;
                continue;
            }
        }

        if (!std::getline(ss, portada, '|')) {
            albumesConErrores++;
            continue;
        }

        idStr.erase(0, idStr.find_first_not_of(" \t\n\r\f\v"));
        idStr.erase(idStr.find_last_not_of(" \t\n\r\f\v") + 1);
        nombre.erase(0, nombre.find_first_not_of(" \t\n\r\f\v"));
        nombre.erase(nombre.find_last_not_of(" \t\n\r\f\v") + 1);
        portada.erase(0, portada.find_first_not_of(" \t\n\r\f\v"));
        portada.erase(portada.find_last_not_of(" \t\n\r\f\v") + 1);

        try {

            int id = std::stoi(idStr);
            int artistaId = id / 100;
            Artista* artista = buscarArtista(artistaId);

            if (artista != nullptr) {
                Album* nuevoAlbum = new Album(id, nombre, artista);
                nuevoAlbum->portada = portada;

                if (agregarAlbum(nuevoAlbum)) {
                    albumesCargados++;
                } else {
                    delete nuevoAlbum;
                    albumesConErrores++;
                }
            } else {
                std::cout << "ERROR linea " << lineNumber << ": Artista no encontrado para album ID: " << id << std::endl;
                albumesConErrores++;
            }

        } catch (const std::exception& e) {
            std::cout << "ERROR linea " << lineNumber << ": " << e.what() << std::endl;
            albumesConErrores++;
            continue;
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

        if (idStr.length() != 9) {
            std::cout << "ERROR: ID de canción debe tener 9 dígitos: " << idStr << std::endl;
            continue;
        }

        int artistaId = id / 10000;
        int albumId = (id % 10000) / 100;
        int cancionId = id % 100;

        int albumIdCompleto = artistaId * 100 + albumId;

        auto validarRutaLinux = [](const std::string& ruta) -> bool {
            if (ruta.empty()) return false;

            if (ruta[0] != '/') {
                std::cout << "ADVERTENCIA: Ruta no es absoluta: " << ruta << std::endl;
                return false;
            }

            return true;
        };

        if (!validarRutaLinux(ruta128) || !validarRutaLinux(ruta320)) {
            std::cout << "ADVERTENCIA: Rutas con formato incorrecto para canción ID: " << id << std::endl;
        }

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
            std::cout << "ERROR: Álbum no encontrado para canción ID: " << id
                      << " (Artista: " << artistaId << ", Álbum: " << albumIdCompleto << ")" << std::endl;
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

unsigned long GestorCatalogo::calcularMemoriaArtistas() const {
    unsigned long memoria = 0;
    ContenedorArtista* actual = inicioArtistas;
    while (actual != nullptr) {
        memoria += sizeof(ContenedorArtista);
        memoria += sizeof(Artista);
        memoria += actual->contenido->nombre.capacity();
        actual = actual->siguiente;
    }
    return memoria;
}

unsigned long GestorCatalogo::calcularMemoriaAlbumes() const {
    unsigned long memoria = 0;
    ContenedorAlbum* actual = inicioAlbumes;
    while (actual != nullptr) {
        memoria += sizeof(ContenedorAlbum);
        memoria += sizeof(Album);
        memoria += actual->contenido->nombre.capacity();
        memoria += actual->contenido->portada.capacity();
        actual = actual->siguiente;
    }
    return memoria;
}

unsigned long GestorCatalogo::calcularMemoriaCanciones() const {
    unsigned long memoria = 0;
    ContenedorCancion* actual = inicioCanciones;
    while (actual != nullptr) {
        memoria += sizeof(ContenedorCancion);
        memoria += sizeof(Cancion);
        memoria += actual->contenido->getNombre().capacity();
        memoria += actual->contenido->obtenerRuta(false).capacity();
        memoria += actual->contenido->obtenerRuta(true).capacity();
        actual = actual->siguiente;
    }
    return memoria;
}
