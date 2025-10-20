#include "SistemaUdeATunes.h"
#include "Usuario.h"
#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include "MensajePublicitario.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

SistemaUdeATunes::SistemaUdeATunes()
    : usuarios(nullptr), artistas(nullptr), albumes(nullptr),
    canciones(nullptr), mensajes(nullptr), usuarioActual(nullptr),
    totalUsuarios(0), totalArtistas(0), totalAlbumes(0),
    totalCanciones(0), totalMensajes(0), capacidad(10) {

    usuarios = new Usuario*[capacidad];
    artistas = new Artista*[capacidad];
    albumes = new Album*[capacidad];
    canciones = new Cancion*[capacidad];
    mensajes = new MensajePublicitario*[capacidad];

    cargarDatos();
}

SistemaUdeATunes::~SistemaUdeATunes() {
    guardarDatos();

    for (int i = 0; i < totalUsuarios; i++) delete usuarios[i];
    for (int i = 0; i < totalArtistas; i++) delete artistas[i];
    for (int i = 0; i < totalAlbumes; i++) delete albumes[i];
    for (int i = 0; i < totalCanciones; i++) delete canciones[i];
    for (int i = 0; i < totalMensajes; i++) delete mensajes[i];

    delete[] usuarios;
    delete[] artistas;
    delete[] albumes;
    delete[] canciones;
    delete[] mensajes;
}

void SistemaUdeATunes::redimensionarUsuarios() {
    int nuevaCapacidad = capacidad * 2;
    Usuario** nuevoArray = new Usuario*[nuevaCapacidad];

    for (int i = 0; i < totalUsuarios; i++) {
        nuevoArray[i] = usuarios[i];
    }

    delete[] usuarios;
    usuarios = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarArtistas() {
    int nuevaCapacidad = capacidad * 2;
    Artista** nuevoArray = new Artista*[nuevaCapacidad];

    for (int i = 0; i < totalArtistas; i++) {
        nuevoArray[i] = artistas[i];
    }

    delete[] artistas;
    artistas = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarAlbumes() {
    int nuevaCapacidad = capacidad * 2;
    Album** nuevoArray = new Album*[nuevaCapacidad];

    for (int i = 0; i < totalAlbumes; i++) {
        nuevoArray[i] = albumes[i];
    }

    delete[] albumes;
    albumes = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarCanciones() {
    int nuevaCapacidad = capacidad * 2;
    Cancion** nuevoArray = new Cancion*[nuevaCapacidad];

    for (int i = 0; i < totalCanciones; i++) {
        nuevoArray[i] = canciones[i];
    }

    delete[] canciones;
    canciones = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::redimensionarMensajes() {
    int nuevaCapacidad = capacidad * 2;
    MensajePublicitario** nuevoArray = new MensajePublicitario*[nuevaCapacidad];

    for (int i = 0; i < totalMensajes; i++) {
        nuevoArray[i] = mensajes[i];
    }

    delete[] mensajes;
    mensajes = nuevoArray;
    capacidad = nuevaCapacidad;
}

void SistemaUdeATunes::cargarUsuarios() {
    std::ifstream archivo("datos/usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de usuarios." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string nickname, membresia, ciudad, pais, fecha;

        std::getline(ss, nickname, '|');
        std::getline(ss, membresia, '|');
        std::getline(ss, ciudad, '|');
        std::getline(ss, pais, '|');
        std::getline(ss, fecha, '|');

        Usuario* nuevoUsuario = new Usuario(nickname, membresia);
        nuevoUsuario->setCiudad(ciudad);
        nuevoUsuario->setPais(pais);
        nuevoUsuario->setFechaInscripcion(fecha);

        agregarUsuario(nuevoUsuario);
    }

    archivo.close();
    std::cout << "Usuarios cargados: " << totalUsuarios << std::endl;
}

void SistemaUdeATunes::cargarArtistas() {
    std::ifstream archivo("datos/artistas.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de artistas." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, nombre, edadStr, pais, seguidoresStr, posicionStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, nombre, '|');
        std::getline(ss, edadStr, '|');
        std::getline(ss, pais, '|');
        std::getline(ss, seguidoresStr, '|');
        std::getline(ss, posicionStr, '|');

        int id = std::stoi(idStr);
        int edad = std::stoi(edadStr);
        int seguidores = std::stoi(seguidoresStr);
        int posicion = std::stoi(posicionStr);

        Artista* nuevoArtista = new Artista(id, nombre);
        nuevoArtista->setSeguidores(seguidores);
        nuevoArtista->setPosicion(posicion);

        agregarArtista(nuevoArtista);
    }

    archivo.close();
    std::cout << "Artistas cargados: " << totalArtistas << std::endl;
}

void SistemaUdeATunes::cargarAlbumes() {
    std::ifstream archivo("datos/albumes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de álbumes." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, nombre, fecha, duracionStr, sello, portada, puntuacionStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, nombre, '|');
        std::getline(ss, fecha, '|');
        std::getline(ss, duracionStr, '|');
        std::getline(ss, sello, '|');
        std::getline(ss, portada, '|');
        std::getline(ss, puntuacionStr, '|');

        int id = std::stoi(idStr);
        float duracion = std::stof(duracionStr);
        float puntuacion = std::stof(puntuacionStr);
        int artistaId = id / 100;
        Artista* artista = buscarArtista(artistaId);

        if (artista != nullptr) {
            Album* nuevoAlbum = new Album(id, nombre, artista);
            nuevoAlbum->setDuracion(duracion);
            nuevoAlbum->setPortada(portada);
            std::string genero;
            while (std::getline(ss, genero, '|')) {
                if (!genero.empty()) {
                    nuevoAlbum->agregarGenero(genero);
                }
            }

            agregarAlbum(nuevoAlbum);
        }
    }

    archivo.close();
    std::cout << "Álbumes cargados: " << totalAlbumes << std::endl;
}

void SistemaUdeATunes::cargarCanciones() {
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

            agregarCancion(nuevaCancion);
        }
    }

    archivo.close();
    std::cout << "Canciones cargadas: " << totalCanciones << std::endl;
}

void SistemaUdeATunes::cargarMensajes() {
    std::ifstream archivo("datos/mensajes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de mensajes." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t pos = linea.find_last_of('|');
        if (pos == std::string::npos) continue;

        std::string texto = linea.substr(0, pos);
        char categoria = linea[pos + 1];

        MensajePublicitario* nuevoMensaje = new MensajePublicitario(texto, categoria);
        agregarMensaje(nuevoMensaje);
    }

    archivo.close();
    std::cout << "Mensajes cargados: " << totalMensajes << std::endl;
}

void SistemaUdeATunes::guardarUsuarios() {
    std::ofstream archivo("datos/usuarios.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de usuarios para guardar." << std::endl;
        return;
    }

    for (int i = 0; i < totalUsuarios; i++) {
        archivo << usuarios[i]->getNickname() << "|"
                << usuarios[i]->getMembresia() << "|"
                << usuarios[i]->getCiudad() << "|"
                << usuarios[i]->getPais() << "|"
                << "2025-10-15" << std::endl; // Fecha hardcodeada por simplicidad
    }

    archivo.close();
}

void SistemaUdeATunes::guardarArtistas() {
    std::ofstream archivo("datos/artistas.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de artistas para guardar." << std::endl;
        return;
    }

    for (int i = 0; i < totalArtistas; i++) {
        archivo << artistas[i]->getId() << "|"
                << artistas[i]->getNombre() << "|"
                << "47" << "|" // Edad hardcodeada
                << "Colombia" << "|" // País hardcodeado
                << artistas[i]->getSeguidores() << "|"
                << artistas[i]->getPosicion() << std::endl;
    }

    archivo.close();
}

void SistemaUdeATunes::guardarAlbumes() {
    // Implementación similar para álbumes
    std::cout << "Guardado de álbumes - Funcionalidad en desarrollo" << std::endl;
}

void SistemaUdeATunes::guardarCanciones() {
    // Implementación similar para canciones
    std::cout << "Guardado de canciones - Funcionalidad en desarrollo" << std::endl;
}

void SistemaUdeATunes::guardarMensajes() {
    // Implementación similar para mensajes
    std::cout << "Guardado de mensajes - Funcionalidad en desarrollo" << std::endl;
}

void SistemaUdeATunes::cargarDatos() {
    std::cout << "=== CARGANDO DATOS DEL SISTEMA ===" << std::endl;
    cargarUsuarios();
    cargarArtistas();
    cargarAlbumes();
    cargarCanciones();
    cargarMensajes();
    std::cout << "=== CARGA DE DATOS COMPLETADA ===" << std::endl;
}

void SistemaUdeATunes::guardarDatos() {
    std::cout << "=== GUARDANDO DATOS DEL SISTEMA ===" << std::endl;
    guardarUsuarios();
    guardarArtistas();
    guardarAlbumes();
    guardarCanciones();
    guardarMensajes();
    std::cout << "=== GUARDADO DE DATOS COMPLETADO ===" << std::endl;
}

bool SistemaUdeATunes::login() {
    std::string nickname;
    std::cout << "Ingrese su nickname: ";
    std::cin >> nickname;

    Usuario* usuario = buscarUsuario(nickname);
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
    if (totalCanciones == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        return;
    }

    std::cout << "Reproducción aleatoria - Funcionalidad en desarrollo" << std::endl;
    std::cout << "Canciones disponibles: " << totalCanciones << std::endl;
}

void SistemaUdeATunes::mostrarMetricas() const {
    std::cout << "=== MÉTRICAS DEL SISTEMA ===" << std::endl;
    std::cout << "Usuarios registrados: " << totalUsuarios << std::endl;
    std::cout << "Artistas en plataforma: " << totalArtistas << std::endl;
    std::cout << "Álbumes disponibles: " << totalAlbumes << std::endl;
    std::cout << "Canciones en catálogo: " << totalCanciones << std::endl;
    std::cout << "Mensajes publicitarios: " << totalMensajes << std::endl;
}

Usuario* SistemaUdeATunes::buscarUsuario(const std::string& nickname) const {
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i]->getNickname() == nickname) {
            return usuarios[i];
        }
    }
    return nullptr;
}

Cancion* SistemaUdeATunes::buscarCancion(int id) const {
    for (int i = 0; i < totalCanciones; i++) {
        if (canciones[i]->getId() == id) {
            return canciones[i];
        }
    }
    return nullptr;
}

Artista* SistemaUdeATunes::buscarArtista(int id) const {
    for (int i = 0; i < totalArtistas; i++) {
        if (artistas[i]->getId() == id) {
            return artistas[i];
        }
    }
    return nullptr;
}

Album* SistemaUdeATunes::buscarAlbum(int id) const {
    for (int i = 0; i < totalAlbumes; i++) {
        if (albumes[i]->getId() == id) {
            return albumes[i];
        }
    }
    return nullptr;
}

bool SistemaUdeATunes::agregarUsuario(Usuario* usuario) {
    if (usuario == nullptr || buscarUsuario(usuario->getNickname()) != nullptr) {
        return false;
    }

    if (totalUsuarios >= capacidad) {
        redimensionarUsuarios();
    }

    usuarios[totalUsuarios++] = usuario;
    return true;
}

bool SistemaUdeATunes::agregarArtista(Artista* artista) {
    if (artista == nullptr) return false;

    if (totalArtistas >= capacidad) {
        redimensionarArtistas();
    }

    artistas[totalArtistas++] = artista;
    return true;
}

bool SistemaUdeATunes::agregarAlbum(Album* album) {
    if (album == nullptr) return false;

    if (totalAlbumes >= capacidad) {
        redimensionarAlbumes();
    }

    albumes[totalAlbumes++] = album;
    return true;
}

bool SistemaUdeATunes::agregarCancion(Cancion* cancion) {
    if (cancion == nullptr) return false;

    if (totalCanciones >= capacidad) {
        redimensionarCanciones();
    }

    canciones[totalCanciones++] = cancion;
    return true;
}

bool SistemaUdeATunes::agregarMensaje(MensajePublicitario* mensaje) {
    if (mensaje == nullptr) return false;

    if (totalMensajes >= capacidad) {
        redimensionarMensajes();
    }

    mensajes[totalMensajes++] = mensaje;
    return true;
}
