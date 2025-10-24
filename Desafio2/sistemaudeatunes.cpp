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
        reproductor = nullptr;
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
    limpiarMensajes();

    std::ifstream archivo("datos/mensajes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de mensajes publicitarios." << std::endl;
        return;
    }

    std::string linea;
    int mensajesCargados = 0;

    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t pos = linea.find_last_of('|');
        if (pos == std::string::npos) {
            std::cout << "Formato incorrecto en mensaje: " << linea << std::endl;
            continue;
        }

        std::string texto = linea.substr(0, pos);
        char categoria = (pos + 1 < linea.length()) ? linea[pos + 1] : 'C';

        texto.erase(0, texto.find_first_not_of(" \t\n\r\f\v"));
        texto.erase(texto.find_last_not_of(" \t\n\r\f\v") + 1);

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
        mensajesCargados++;
    }

    archivo.close();
    //std::cout << "Mensajes publicitarios cargados: " << mensajesCargados << std::endl;
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
    resetIteraciones();

    gestorUsuarios->cargarUsuarios();
    incrementarIteraciones(gestorUsuarios->getIteraciones());

    gestorCatalogo->cargarArtistas();
    incrementarIteraciones(gestorCatalogo->getIteraciones());

    gestorCatalogo->cargarAlbumes();
    incrementarIteraciones(gestorCatalogo->getIteraciones());

    gestorCatalogo->cargarCanciones();
    incrementarIteraciones(gestorCatalogo->getIteraciones());

    cargarMensajes();

}

bool SistemaUdeATunes::login() {

    std::string nickname;
    std::cout << "Ingrese su nickname: ";
    std::cin >> nickname;
    incrementarIteraciones();

    Usuario* usuario = gestorUsuarios->buscarUsuario(nickname);
    unsigned long iteracionesBusqueda = gestorUsuarios->getIteraciones();
    incrementarIteraciones(iteracionesBusqueda + 1);

    if (usuario != nullptr) {
        usuarioActual = usuario;
        std::cout << "Bienvenido " << nickname << std::endl;
        std::cout << "Membresia: " << usuario->getMembresia() << std::endl;
        incrementarIteraciones(2);
        return true;
    } else {
        std::cout << "Usuario no encontrado." << std::endl;
        incrementarIteraciones();
        return false;
    }
}

void SistemaUdeATunes::reproducirAleatorio() {
    resetIteraciones();

    if (gestorCatalogo->getTotalCanciones() == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        incrementarIteraciones(2);
        return;
    }

    if (usuarioActual == nullptr) {
        std::cout << "Debe iniciar sesion para reproducir musica." << std::endl;
        incrementarIteraciones(2);
        return;
    }

    incrementarIteraciones(3);

    Cancion** cancionesArray = gestorCatalogo->getCancionesArray();
    MensajePublicitario** mensajesArray = getMensajesArray();
    incrementarIteraciones(2);

    if (cancionesArray == nullptr) {
        std::cout << "ERROR: No se pudieron cargar las canciones" << std::endl;
        if (mensajesArray != nullptr) delete[] mensajesArray;
        incrementarIteraciones(2);
        return;
    }

    if (reproductor == nullptr) {
        reproductor = new Reproductor(
            cancionesArray,
            gestorCatalogo->getTotalCanciones(),
            mensajesArray,
            totalMensajes,
            usuarioActual
            );
    } else {
        reproductor->actualizarDatos(
            cancionesArray,
            gestorCatalogo->getTotalCanciones(),
            mensajesArray,
            totalMensajes,
            usuarioActual
            );
    }

    reproductor->reproducirAleatorio();

    delete[] cancionesArray;
    delete[] mensajesArray;

    std::cout << "\n--- METRICAS DE EFICIENCIA ---" << std::endl;
    std::cout << "Iteraciones realizadas: " << getTotalIteraciones() << std::endl;
    calcularMemoria();
    std::cout << "Memoria consumida: " << memoriaConsumida << " bytes ("
              << memoriaConsumida / 1024 << " KB)" << std::endl;
}

void SistemaUdeATunes::reproducirListaFavoritos(bool ordenAleatorio) {
    resetIteraciones();

    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden reproducir listas de favoritos." << std::endl;
        incrementarIteraciones(2);
        return;
    }

    if (usuarioActual->getListaFavoritos() == nullptr) {
        std::cout << "No tienes lista de favoritos." << std::endl;
        incrementarIteraciones(3);
        return;
    }

    int totalCancionesFavoritas = usuarioActual->getListaFavoritos()->getTotalCancionesVisibles();
    if (totalCancionesFavoritas == 0) {
        std::cout << "Tu lista de favoritos esta vacia." << std::endl;
        incrementarIteraciones(4);
        return;
    }

    incrementarIteraciones(4);

    Cancion** cancionesFavoritas = usuarioActual->getListaFavoritos()->getCancionesConSeguidas(ordenAleatorio);
    MensajePublicitario** mensajesArray = getMensajesArray();

    if (cancionesFavoritas == nullptr) {
        std::cout << "Error al cargar las canciones de favoritos." << std::endl;
        if (mensajesArray != nullptr) delete[] mensajesArray;
        incrementarIteraciones(5);
        return;
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "       REPRODUCIENDO LISTA DE FAVORITOS" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalCancionesFavoritas << std::endl;
    std::cout << " - Propias: " << usuarioActual->getListaFavoritos()->getTotalCanciones() << std::endl;
    std::cout << " - Seguidas: " << (usuarioActual->getListaFavoritos()->estaSiguiendoLista() ?
                                         usuarioActual->getListaFavoritos()->getTotalCancionesVisibles() -
                                             usuarioActual->getListaFavoritos()->getTotalCanciones() : 0) << std::endl;
    std::cout << "Modo: " << (ordenAleatorio ? "Aleatorio" : "Orden original") << std::endl;
    std::cout << "==========================================" << std::endl;

    if (reproductor == nullptr) {
        reproductor = new Reproductor(
            cancionesFavoritas,
            totalCancionesFavoritas,
            mensajesArray,
            totalMensajes,
            usuarioActual
            );
    } else {
        reproductor->actualizarDatos(
            cancionesFavoritas,
            totalCancionesFavoritas,
            mensajesArray,
            totalMensajes,
            usuarioActual
            );
    }

    reproductor->reproducirAleatorio();

    unsigned long iteracionesFavoritos = usuarioActual->getListaFavoritos()->getIteraciones();
    incrementarIteraciones(iteracionesFavoritos + 5);

    std::cout << "\n--- METRICAS DE EFICIENCIA ---" << std::endl;
    std::cout << "Iteraciones realizadas: " << getTotalIteraciones() << std::endl;
    std::cout << " - Sistema: " << totalIteraciones - iteracionesFavoritos << std::endl;
    std::cout << " - ListaFavoritos: " << iteracionesFavoritos << std::endl;
    calcularMemoria();
    std::cout << "Memoria consumida: " << memoriaConsumida << " bytes ("
              << memoriaConsumida / 1024 << " KB)" << std::endl;

    delete[] cancionesFavoritas;
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

void SistemaUdeATunes::mostrarCancionesDisponibles() const {
    resetIteraciones();

    int totalCanciones = gestorCatalogo->getTotalCanciones();
    incrementarIteraciones();

    Cancion** cancionesArray = gestorCatalogo->getCancionesArray();
    unsigned long iteracionesArray = gestorCatalogo->getIteraciones();
    incrementarIteraciones(iteracionesArray + 1);

    std::cout << "\n==========================================" << std::endl;
    std::cout << "          CATALOGO DE CANCIONES" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalCanciones << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    incrementarIteraciones(5);

    for (int i = 0; i < totalCanciones; i++) {
        std::cout << "ID: " << cancionesArray[i]->getId()
        << " | " << cancionesArray[i]->getNombre()
        << " - " << cancionesArray[i]->getAlbum()->artista->nombre << std::endl;
        incrementarIteraciones();
    }

    std::cout << "==========================================" << std::endl;
    incrementarIteraciones();

    delete[] cancionesArray;
}

bool SistemaUdeATunes::agregarCancionAFavoritos(int idCancion) {
    resetIteraciones(); // Solo resetear al inicio de esta operación

    if (usuarioActual == nullptr || !usuarioActual->esPremium()) {
        std::cout << "Solo usuarios premium pueden tener listas de favoritos." << std::endl;
        incrementarIteraciones(2);
        return false;
    }

    incrementarIteraciones(2); // Verificación de usuario

    Cancion* cancion = gestorCatalogo->buscarCancion(idCancion);
    unsigned long iteracionesBusqueda = gestorCatalogo->getIteraciones();
    incrementarIteraciones(iteracionesBusqueda + 1);

    if (cancion == nullptr) {
        std::cout << "Cancion no encontrada." << std::endl;
        incrementarIteraciones();
        return false;
    }

    incrementarIteraciones(); // Canción encontrada

    bool resultado = usuarioActual->getListaFavoritos()->agregarCancion(cancion);
    unsigned long iteracionesFavoritos = usuarioActual->getListaFavoritos()->getIteraciones();
    incrementarIteraciones(iteracionesFavoritos);

    if (resultado) {
        std::cout << "Cancion agregada a favoritos: " << cancion->getNombre() << std::endl;
        incrementarIteraciones();
    } else {
        std::cout << "La cancion ya esta en favoritos o la lista esta llena." << std::endl;
        incrementarIteraciones();
    }

    std::cout << "\n--- METRICAS DE EFICIENCIA ---" << std::endl;
    std::cout << "Iteraciones realizadas: " << totalIteraciones << std::endl;
    std::cout << " - Sistema: " << totalIteraciones - iteracionesFavoritos << std::endl;
    std::cout << " - ListaFavoritos: " << iteracionesFavoritos << std::endl;
    calcularMemoria();
    std::cout << "Memoria consumida: " << memoriaConsumida << " bytes ("
              << memoriaConsumida / 1024 << " KB)" << std::endl;

    return resultado;
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

void SistemaUdeATunes::incrementarIteraciones(int cantidad) const {
    totalIteraciones += cantidad;
}

void SistemaUdeATunes::resetIteraciones() const {
    totalIteraciones = 0;
    gestorUsuarios->resetIteraciones();
    gestorCatalogo->resetIteraciones();
}

unsigned long SistemaUdeATunes::getTotalIteraciones() const {
    unsigned long totalReproductor = (reproductor != nullptr) ? reproductor->getIteraciones() : 0;
    return totalIteraciones + gestorUsuarios->getIteraciones() + gestorCatalogo->getIteraciones() + totalReproductor;
}

unsigned long SistemaUdeATunes::calcularMemoriaMensajes() const {
    unsigned long memoria = 0;
    ContenedorMensaje* actual = inicioMensajes;
    while (actual != nullptr) {
        memoria += sizeof(ContenedorMensaje);
        memoria += sizeof(MensajePublicitario);
        memoria += actual->contenido->texto.capacity();
        actual = actual->siguiente;
    }
    return memoria;
}

unsigned long SistemaUdeATunes::calcularMemoriaTodasListasFavoritos() const {
    unsigned long memoria = 0;

    Usuario** usuariosArray = gestorUsuarios->getUsuariosArray();
    if (usuariosArray != nullptr) {
        int totalUsuarios = gestorUsuarios->getTotalUsuarios();
        for (int i = 0; i < totalUsuarios; i++) {
            if (usuariosArray[i]->getListaFavoritos() != nullptr) {
                memoria += sizeof(ListaFavoritos);
                int totalCanciones = usuariosArray[i]->getListaFavoritos()->getTotalCanciones();
                memoria += totalCanciones * sizeof(void*); // Aproximación de nodos
            }
        }
        delete[] usuariosArray;
    }

    return memoria;
}

void SistemaUdeATunes::calcularMemoria() const {
    memoriaConsumida = 0;

    memoriaConsumida += sizeof(*this);
    memoriaConsumida += sizeof(*gestorUsuarios);
    memoriaConsumida += sizeof(*gestorCatalogo);

    if (reproductor != nullptr) {
        memoriaConsumida += sizeof(*reproductor);
    }

    if (usuarioActual != nullptr) {
        memoriaConsumida += sizeof(*usuarioActual);
        memoriaConsumida += usuarioActual->getNickname().capacity();
        memoriaConsumida += usuarioActual->getMembresia().capacity();
    }

    memoriaConsumida += gestorUsuarios->calcularMemoriaUsuarios();

    memoriaConsumida += gestorCatalogo->calcularMemoriaArtistas();
    memoriaConsumida += gestorCatalogo->calcularMemoriaAlbumes();
    memoriaConsumida += gestorCatalogo->calcularMemoriaCanciones();

    memoriaConsumida += calcularMemoriaMensajes();

    memoriaConsumida += calcularMemoriaTodasListasFavoritos();
}

void SistemaUdeATunes::mostrarMetricasEficiencia() const {
    calcularMemoria();

    unsigned long totalGestorUsuarios = gestorUsuarios->getIteraciones();
    unsigned long totalGestorCatalogo = gestorCatalogo->getIteraciones();
    unsigned long totalSistema = totalIteraciones;
    unsigned long totalReproductor = (reproductor != nullptr) ? reproductor->getIteraciones() : 0;

    std::cout << "\n==========================================" << std::endl;
    std::cout << "          METRICAS DE EFICIENCIA" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de iteraciones: " << getTotalIteraciones() << std::endl;
    std::cout << " - GestorUsuarios: " << totalGestorUsuarios << std::endl;
    std::cout << " - GestorCatalogo: " << totalGestorCatalogo << std::endl;
    std::cout << " - Sistema: " << totalSistema << std::endl;
    std::cout << " - Reproductor: " << totalReproductor << std::endl;
    std::cout << "Memoria consumida: " << memoriaConsumida << " bytes" << std::endl;
    std::cout << "Aproximadamente: " << memoriaConsumida / 1024 << " KB" << std::endl;
    std::cout << "Aproximadamente: " << memoriaConsumida / (1024 * 1024) << " MB" << std::endl;

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Usuarios en sistema: " << gestorUsuarios->getTotalUsuarios() << std::endl;
    std::cout << "Canciones en sistema: " << gestorCatalogo->getTotalCanciones() << std::endl;
    std::cout << "Artistas en sistema: " << gestorCatalogo->getTotalArtistas() << std::endl;
    std::cout << "Albumes en sistema: " << gestorCatalogo->getTotalAlbumes() << std::endl;
    std::cout << "Mensajes publicitarios: " << totalMensajes << std::endl;
    std::cout << "==========================================" << std::endl;
}
