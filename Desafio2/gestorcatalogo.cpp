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
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        ContenedorArtista* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    incrementarIteraciones(iteracionesLocales);

    inicioArtistas = nullptr;
    totalArtistas = 0;
}

void GestorCatalogo::limpiarAlbumes() {
    ContenedorAlbum* actual = inicioAlbumes;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        ContenedorAlbum* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    incrementarIteraciones(iteracionesLocales);

    inicioAlbumes = nullptr;
    totalAlbumes = 0;
}

void GestorCatalogo::limpiarCanciones() {
    ContenedorCancion* actual = inicioCanciones;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        ContenedorCancion* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    incrementarIteraciones(iteracionesLocales);

    inicioCanciones = nullptr;
    totalCanciones = 0;
}

void GestorCatalogo::limpiarCatalogo() {
    limpiarCanciones();
    limpiarAlbumes();
    limpiarArtistas();
}

bool GestorCatalogo::agregarArtista(Artista* artista) {
    resetIteraciones();

    if (artista == nullptr) {
        incrementarIteraciones();
        return false;
    }

    if (buscarArtista(artista->id) != nullptr) {
        incrementarIteraciones(2);
        return false;
    }

    ContenedorArtista* nuevoContenedor = new ContenedorArtista(artista);
    incrementarIteraciones();

    if (inicioArtistas == nullptr) {
        inicioArtistas = nuevoContenedor;
        incrementarIteraciones();
    } else {
        ContenedorArtista* actual = inicioArtistas;
        int iteracionesLocales = 0;

        while (actual->siguiente != nullptr) {
            iteracionesLocales++;
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
        incrementarIteraciones(iteracionesLocales + 1);
    }

    totalArtistas++;
    incrementarIteraciones();
    return true;
}

bool GestorCatalogo::agregarAlbum(Album* album) {
    resetIteraciones();

    if (album == nullptr) {
        incrementarIteraciones();
        return false;
    }

    if (buscarAlbum(album->id) != nullptr) {
        incrementarIteraciones(2);
        return false;
    }

    ContenedorAlbum* nuevoContenedor = new ContenedorAlbum(album);
    incrementarIteraciones();

    if (inicioAlbumes == nullptr) {
        inicioAlbumes = nuevoContenedor;
        incrementarIteraciones();
    } else {
        ContenedorAlbum* actual = inicioAlbumes;
        int iteracionesLocales = 0;

        while (actual->siguiente != nullptr) {
            iteracionesLocales++;
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
        incrementarIteraciones(iteracionesLocales + 1);
    }

    totalAlbumes++;
    incrementarIteraciones();
    return true;
}

bool GestorCatalogo::agregarCancion(Cancion* cancion) {
    resetIteraciones();

    if (cancion == nullptr) {
        incrementarIteraciones();
        return false;
    }

    if (buscarCancion(cancion->getId()) != nullptr) {
        incrementarIteraciones(2);
        return false;
    }

    ContenedorCancion* nuevoContenedor = new ContenedorCancion(cancion);
    incrementarIteraciones();

    if (inicioCanciones == nullptr) {
        inicioCanciones = nuevoContenedor;
        incrementarIteraciones();
    } else {
        ContenedorCancion* actual = inicioCanciones;
        int iteracionesLocales = 0;

        while (actual->siguiente != nullptr) {
            iteracionesLocales++;
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoContenedor;
        incrementarIteraciones(iteracionesLocales + 1);
    }

    totalCanciones++;
    incrementarIteraciones();
    return true;
}

Artista* GestorCatalogo::buscarArtista(int id) const {
    resetIteraciones();

    ContenedorArtista* actual = inicioArtistas;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        if (actual->contenido->id == id) {
            incrementarIteraciones(iteracionesLocales);
            return actual->contenido;
        }
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return nullptr;
}

Album* GestorCatalogo::buscarAlbum(int id) const {
    resetIteraciones();

    ContenedorAlbum* actual = inicioAlbumes;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        if (actual->contenido->id == id) {
            incrementarIteraciones(iteracionesLocales);
            return actual->contenido;
        }
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return nullptr;
}

Cancion* GestorCatalogo::buscarCancion(int id) const {
    resetIteraciones();

    ContenedorCancion* actual = inicioCanciones;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        if (actual->contenido->getId() == id) {
            incrementarIteraciones(iteracionesLocales);
            return actual->contenido;
        }
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return nullptr;
}

Cancion** GestorCatalogo::getCancionesArray() const {
    resetIteraciones();

    if (totalCanciones == 0) {
        incrementarIteraciones();
        return nullptr;
    }

    Cancion** array = new Cancion*[totalCanciones];
    ContenedorCancion* actual = inicioCanciones;
    int index = 0;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales + 1);
    return array;
}

void GestorCatalogo::cargarArtistas() {
    resetIteraciones();

    std::ifstream archivo("datos/artistas.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de artistas." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::string linea;
    int lineNumber = 0;
    int artistasCargados = 0;
    int iteracionesLocales = 0;

    while (std::getline(archivo, linea)) {
        iteracionesLocales++;
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
                    iteracionesLocales += getIteraciones();
                } else {
                    delete nuevoArtista;
                }
                resetIteraciones();

            } catch (const std::exception& e) {
                std::cout << "ERROR linea " << lineNumber << ": " << e.what() << std::endl;
                continue;
            }
        }
    }

    archivo.close();
    incrementarIteraciones(iteracionesLocales + 1);
}

void GestorCatalogo::cargarAlbumes() {
    resetIteraciones();

    std::ifstream archivo("datos/albumes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de albumes." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::string linea;
    int lineNumber = 0;
    int albumesCargados = 0;
    int albumesConErrores = 0;
    int iteracionesLocales = 0;

    while (std::getline(archivo, linea)) {
        iteracionesLocales++;
        lineNumber++;
        if (linea.empty()) continue;

        int totalCampos = 0;
        for (char c : linea) {
            iteracionesLocales++;
            if (c == '|') totalCampos++;
        }
        totalCampos++;

        if (totalCampos < 7) {
            std::cout << "ERROR linea " << lineNumber << ": Album necesita al menos 7 campos, tiene " << totalCampos << std::endl;
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
            iteracionesLocales += getIteraciones();

            if (artista != nullptr) {
                Album* nuevoAlbum = new Album(id, nombre, artista);
                nuevoAlbum->portada = portada;

                if (agregarAlbum(nuevoAlbum)) {
                    albumesCargados++;
                    iteracionesLocales += getIteraciones();
                } else {
                    delete nuevoAlbum;
                    albumesConErrores++;
                }
                resetIteraciones();
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
    incrementarIteraciones(iteracionesLocales + 1);
}

void GestorCatalogo::cargarCanciones() {
    resetIteraciones();

    std::ifstream archivo("datos/canciones.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de canciones." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::string linea;
    int lineNumber = 0;
    int cancionesCargadas = 0;
    int iteracionesLocales = 0;

    while (std::getline(archivo, linea)) {
        iteracionesLocales++;
        lineNumber++;
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
        iteracionesLocales += getIteraciones();

        if (album != nullptr && album->artista->id == artistaId) {
            Cancion* nuevaCancion = new Cancion(id, nombre, album);
            nuevaCancion->setDuracion(std::stof(duracionStr));
            nuevaCancion->setRuta128(ruta128);
            nuevaCancion->setRuta320(ruta320);

            int reproducciones = std::stoi(reproduccionesStr);
            for (int i = 0; i < reproducciones; i++) {
                nuevaCancion->incrementarReproducciones();
                iteracionesLocales++;
            }

            if (agregarCancion(nuevaCancion)) {
                cancionesCargadas++;
                iteracionesLocales += getIteraciones();
            }
            resetIteraciones();
        } else {
            std::cout << "ERROR: Álbum no encontrado para canción ID: " << id
                      << " (Artista: " << artistaId << ", Álbum: " << albumIdCompleto << ")" << std::endl;
        }
    }

    archivo.close();
    incrementarIteraciones(iteracionesLocales + 1);
}

void GestorCatalogo::guardarCanciones() const {
    resetIteraciones();

    std::ofstream archivo("datos/canciones.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de canciones para guardar." << std::endl;
        incrementarIteraciones();
        return;
    }

    ContenedorCancion* actual = inicioCanciones;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        archivo << actual->contenido->getId() << "|"
                << actual->contenido->getNombre() << "|"
                << actual->contenido->getDuracion() << "|"
                << actual->contenido->obtenerRuta(false) << "|"
                << actual->contenido->obtenerRuta(true) << "|"
                << actual->contenido->getReproducciones() << std::endl;
        actual = actual->siguiente;
    }

    archivo.close();
    incrementarIteraciones(iteracionesLocales + 1);
}

unsigned long GestorCatalogo::calcularMemoriaArtistas() const {
    unsigned long memoria = 0;
    ContenedorArtista* actual = inicioArtistas;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        memoria += sizeof(ContenedorArtista);
        memoria += sizeof(Artista);
        memoria += actual->contenido->nombre.capacity();
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return memoria;
}

unsigned long GestorCatalogo::calcularMemoriaAlbumes() const {
    unsigned long memoria = 0;
    ContenedorAlbum* actual = inicioAlbumes;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        memoria += sizeof(ContenedorAlbum);
        memoria += sizeof(Album);
        memoria += actual->contenido->nombre.capacity();
        memoria += actual->contenido->portada.capacity();
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return memoria;
}

unsigned long GestorCatalogo::calcularMemoriaCanciones() const {
    unsigned long memoria = 0;
    ContenedorCancion* actual = inicioCanciones;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        memoria += sizeof(ContenedorCancion);
        memoria += sizeof(Cancion);
        memoria += actual->contenido->getNombre().capacity();
        memoria += actual->contenido->obtenerRuta(false).capacity();
        memoria += actual->contenido->obtenerRuta(true).capacity();
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return memoria;
}
