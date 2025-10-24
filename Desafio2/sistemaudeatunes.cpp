#include "SistemaUdeATunes.h"
#include "Usuario.h"
#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include "MensajePublicitario.h"
#include "reproductor.h"
#include "ListaFavoritos.h"
#include "GestorUsuarios.h"
#include "GestorCatalogo.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

SistemaUdeATunes::SistemaUdeATunes()
    : gestorUsuarios(new GestorUsuarios()),
    gestorCatalogo(new GestorCatalogo()),
    inicioMensajes(nullptr),
    totalMensajes(0),
    usuarioActual(nullptr),
    reproductor(nullptr),
    totalIteraciones(0),
    memoriaConsumida(0) {
}

SistemaUdeATunes::~SistemaUdeATunes() {
    guardarCambios();

    if (reproductor != nullptr) {
        delete reproductor;
    }

    limpiarMensajes();
    delete gestorCatalogo;
    delete gestorUsuarios;
}

void SistemaUdeATunes::limpiarMensajes() {
    ContenedorMensaje* actual = inicioMensajes;
    while (actual != nullptr) {
        ContenedorMensaje* siguiente = actual->siguiente;
        delete actual->contenido;
        delete actual;
        actual = siguiente;
    }
    inicioMensajes = nullptr;
    totalMensajes = 0;
}

void SistemaUdeATunes::cargarMensajes() {
    std::ifstream archivo("datos/mensajes.txt");
    if (!archivo.is_open()) {
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

        ContenedorMensaje* nuevoContenedor = new ContenedorMensaje(nuevoMensaje);

        if (inicioMensajes == nullptr) {
            inicioMensajes = nuevoContenedor;
        } else {
            ContenedorMensaje* actual = inicioMensajes;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoContenedor;
        }

        totalMensajes++;
    }

    archivo.close();
}

MensajePublicitario** SistemaUdeATunes::getMensajesArray() const {
    if (totalMensajes == 0) return nullptr;

    MensajePublicitario** array = new MensajePublicitario*[totalMensajes];
    ContenedorMensaje* actual = inicioMensajes;
    int index = 0;

    while (actual != nullptr) {
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    return array;
}

void SistemaUdeATunes::guardarCambios() const {
    gestorCatalogo->guardarCanciones();
}

void SistemaUdeATunes::cargarDatos() {

    gestorUsuarios->cargarUsuarios();
    gestorCatalogo->cargarArtistas();
    gestorCatalogo->cargarAlbumes();
    gestorCatalogo->cargarCanciones();
    cargarMensajes();

}

bool SistemaUdeATunes::login() {
    std::string nickname;
    std::cout << "Ingrese su nickname: ";
    std::cin >> nickname;

    Usuario* usuario = gestorUsuarios->buscarUsuario(nickname);
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
    if (gestorCatalogo->getTotalCanciones() == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        return;
    }

    if (usuarioActual == nullptr) {
        std::cout << "Debe iniciar sesion para reproducir musica." << std::endl;
        return;
    }

    Cancion** cancionesArray = gestorCatalogo->getCancionesArray();
    MensajePublicitario** mensajesArray = getMensajesArray();

    Reproductor* nuevoReproductor = new Reproductor(
        cancionesArray,
        gestorCatalogo->getTotalCanciones(),
        mensajesArray,
        totalMensajes,
        usuarioActual
        );

    delete[] cancionesArray;
    delete[] mensajesArray;

    if (reproductor != nullptr) {
        delete reproductor;
    }

    reproductor = nuevoReproductor;
    reproductor->reproducirAleatorio();
}

void SistemaUdeATunes::incrementarIteraciones(int cantidad) const {
    totalIteraciones += cantidad;
}

void SistemaUdeATunes::mostrarMetricas() const {
    std::cout << "\n==========================================" << std::endl;
    std::cout << "          METRICAS DEL SISTEMA" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Usuarios registrados: " << gestorUsuarios->getTotalUsuarios() << std::endl;
    std::cout << "Artistas en plataforma: " << gestorCatalogo->getTotalArtistas() << std::endl;
    std::cout << "Albumes disponibles: " << gestorCatalogo->getTotalAlbumes() << std::endl;
    std::cout << "Canciones en catalogo: " << gestorCatalogo->getTotalCanciones() << std::endl;
    std::cout << "Mensajes publicitarios: " << totalMensajes << std::endl;
    std::cout << "==========================================" << std::endl;
}

Usuario* SistemaUdeATunes::buscarUsuario(const std::string& nickname) const {
    return gestorUsuarios->buscarUsuario(nickname);
}

Cancion* SistemaUdeATunes::buscarCancion(int id) const {
    return gestorCatalogo->buscarCancion(id);
}

Artista* SistemaUdeATunes::buscarArtista(int id) const {
    return gestorCatalogo->buscarArtista(id);
}

Album* SistemaUdeATunes::buscarAlbum(int id) const {
    return gestorCatalogo->buscarAlbum(id);
}

void SistemaUdeATunes::mostrarCancionesDisponibles() const {
    int totalCanciones = gestorCatalogo->getTotalCanciones();

    Cancion** cancionesArray = gestorCatalogo->getCancionesArray();

    std::cout << "\n==========================================" << std::endl;
    std::cout << "          CATALOGO DE CANCIONES" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalCanciones << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    for (int i = 0; i < totalCanciones; i++) {
        std::cout << "ID: " << cancionesArray[i]->getId()
        << " | " << cancionesArray[i]->getNombre()
        << " - " << cancionesArray[i]->getAlbum()->artista->nombre << std::endl;
    }

    std::cout << "==========================================" << std::endl;

    delete[] cancionesArray;
}

bool SistemaUdeATunes::agregarCancionAFavoritos(int idCancion) {
    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden tener listas de favoritos." << std::endl;
        return false;
    }

    Cancion* cancion = gestorCatalogo->buscarCancion(idCancion);
    if (cancion == nullptr) {
        std::cout << "Canción no encontrada." << std::endl;
        return false;
    }

    if (usuarioActual->getListaFavoritos()->agregarCancion(cancion)) {
        std::cout << "Canción agregada a favoritos: " << cancion->getNombre() << std::endl;
        return true;
    } else {
        std::cout << "La canción ya está en favoritos o la lista está llena." << std::endl;
        return false;
    }
}

void SistemaUdeATunes::calcularMemoria() const {

    memoriaConsumida = 0;
    memoriaConsumida += sizeof(*this);
    memoriaConsumida += sizeof(*gestorUsuarios);
    memoriaConsumida += sizeof(*gestorCatalogo);

    ContenedorMensaje* actual = inicioMensajes;
    while (actual != nullptr) {
        memoriaConsumida += sizeof(ContenedorMensaje);
        memoriaConsumida += sizeof(MensajePublicitario);
        memoriaConsumida += actual->contenido->texto.capacity();
        actual = actual->siguiente;
    }
    memoriaConsumida += gestorUsuarios->getTotalUsuarios() * (sizeof(Usuario) + 50);
    memoriaConsumida += gestorCatalogo->getTotalArtistas() * (sizeof(Artista) + 40);
    memoriaConsumida += gestorCatalogo->getTotalAlbumes() * (sizeof(Album) + 100);
    memoriaConsumida += gestorCatalogo->getTotalCanciones() * (sizeof(Cancion) + 80);
}

void SistemaUdeATunes::mostrarMetricasEficiencia() const {
    calcularMemoria();

    std::cout << "\n==========================================" << std::endl;
    std::cout << "          METRICAS DE EFICIENCIA" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de iteraciones: " << totalIteraciones << std::endl;
    std::cout << "Memoria consumida: " << memoriaConsumida << " bytes" << std::endl;
    std::cout << "Aproximadamente: " << memoriaConsumida / 1024 << " KB" << std::endl;
    std::cout << "==========================================" << std::endl;
}

bool SistemaUdeATunes::seguirListaUsuario(const std::string& nicknameSeguido) {
    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden seguir listas." << std::endl;
        return false;
    }

    if (nicknameSeguido == usuarioActual->getNickname()) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        return false;
    }

    incrementarIteraciones();
    Usuario* usuarioSeguido = gestorUsuarios->buscarUsuario(nicknameSeguido);

    if (usuarioSeguido == nullptr) {
        std::cout << "Usuario '" << nicknameSeguido << "' no encontrado." << std::endl;
        return false;
    }

    if (!usuarioSeguido->esPremium()) {
        std::cout << "El usuario '" << nicknameSeguido << "' no es premium y no tiene lista de favoritos." << std::endl;
        return false;
    }

    if (usuarioSeguido->getListaFavoritos() == nullptr) {
        std::cout << "El usuario '" << nicknameSeguido << "' no tiene lista de favoritos." << std::endl;
        return false;
    }

    usuarioActual->getListaFavoritos()->seguirLista(usuarioSeguido->getListaFavoritos());
    return true;
}

void SistemaUdeATunes::dejarDeSeguirLista() {
    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden seguir listas." << std::endl;
        return;
    }

    if (usuarioActual->getListaFavoritos() == nullptr) {
        std::cout << "No tienes lista de favoritos." << std::endl;
        return;
    }

    usuarioActual->getListaFavoritos()->dejarDeSeguirLista();
}
