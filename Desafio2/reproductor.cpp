#include "Reproductor.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include "Usuario.h"
#include "MensajePublicitario.h"
#include <iostream>
#include <chrono>

Reproductor::Reproductor(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario)
    : todasLasCanciones(canciones), todosLosMensajes(mensajes), totalCanciones(totalCanc),
    totalMensajes(totalMsg), usuarioActual(usuario), cancionActual(nullptr),
    indiceActual(-1), reproduciendo(false), modoRepetir(false), indiceHistorial(0),
    contadorCancionesReproducidas(0) {

    for (int i = 0; i < 4; i++) {
        historial[i] = nullptr;
    }

    auto ahora = std::chrono::steady_clock::now();
    auto semilla = ahora.time_since_epoch().count();
    generadorAleatorio.seed(static_cast<unsigned int>(semilla));
}

int Reproductor::generarNumeroAleatorio(int maximo) {
    if (maximo <= 0) return 0;
    std::uniform_int_distribution<int> distribucion(0, maximo - 1);
    return distribucion(generadorAleatorio);
}

void Reproductor::mostrarInterfazReproduccion() {
    if (cancionActual == nullptr) return;

    std::cout << "\n=== REPRODUCIENDO ===" << std::endl;
    std::cout << "Artista: " << cancionActual->getAlbum()->getArtista()->getNombre() << std::endl;
    std::cout << "Álbum: " << cancionActual->getAlbum()->getNombre() << std::endl;
    std::cout << "Portada: " << cancionActual->getAlbum()->getPortada() << std::endl;
    std::cout << "Canción: " << cancionActual->getNombre() << std::endl;
    std::string rutaAudio = cancionActual->obtenerRuta(usuarioActual->esPremium());
    std::cout << "Audio: " << rutaAudio << std::endl;
    std::cout << "Duración: " << cancionActual->getDuracion() << " minutos" << std::endl;
    std::cout << "\n--- OPCIONES ---" << std::endl;
    std::cout << "1. Detener reproducción" << std::endl;
    std::cout << "2. Siguiente canción" << std::endl;

    if (usuarioActual->esPremium()) {
        std::cout << "3. Canción anterior" << std::endl;
        std::cout << "4. " << (modoRepetir ? "Desactivar" : "Activar") << " repetir" << std::endl;
    }

    std::cout << "0. Volver al menú principal" << std::endl;
    std::cout << "=====================" << std::endl;
}

void Reproductor::mostrarPublicidad() {
    if (usuarioActual->esPremium()) return;

    if (contadorCancionesReproducidas % 2 == 0 && totalMensajes > 0) {
        MensajePublicitario* mensaje = obtenerMensajeAleatorio();
        if (mensaje != nullptr) {
            std::cout << "\n--- PUBLICIDAD ---" << std::endl;
            std::cout << mensaje->getTexto() << std::endl;
            std::cout << "Categoría: " << mensaje->getCategoria() << std::endl;
            std::cout << "------------------" << std::endl;

            std::cout << "Publicidad en progreso..." << std::endl;
            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
            }
        }
    }
}

MensajePublicitario* Reproductor::obtenerMensajeAleatorio() {
    if (totalMensajes == 0) return nullptr;

    int pesoTotal = 0;
    for (int i = 0; i < totalMensajes; i++) {
        pesoTotal += todosLosMensajes[i]->getPrioridad();
    }

    int randomValue = generarNumeroAleatorio(pesoTotal);
    int accumulatedWeight = 0;

    for (int i = 0; i < totalMensajes; i++) {
        accumulatedWeight += todosLosMensajes[i]->getPrioridad();
        if (randomValue < accumulatedWeight) {
            return todosLosMensajes[i];
        }
    }

    return todosLosMensajes[0];
}

void Reproductor::reproducirAleatorio() {
    if (totalCanciones == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        return;
    }

    reproduciendo = true;
    contadorCancionesReproducidas = 0;

    std::cout << "Iniciando reproducción aleatoria..." << std::endl;
    std::cout << "Se reproducirán 5 canciones (modo prueba)" << std::endl;

    for (int i = 0; i < 5 && reproduciendo; i++) {
        indiceActual = generarNumeroAleatorio(totalCanciones);
        cancionActual = todasLasCanciones[indiceActual];

        if (usuarioActual->esPremium()) {
            historial[indiceHistorial % 4] = cancionActual;
            indiceHistorial++;
        }

        cancionActual->incrementarReproducciones();
        contadorCancionesReproducidas++;
        mostrarInterfazReproduccion();
        mostrarPublicidad();

        std::cout << "Reproduciendo..." << std::endl;

        auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
        }

        std::cout << "Canción finalizada." << std::endl;

        if (modoRepetir) {
            i--;
            std::cout << "Modo repetir activado - repitiendo canción..." << std::endl;
        }
    }

    if (reproduciendo) {
        std::cout << "Reproducción finalizada (límite de 5 canciones alcanzado)." << std::endl;
    }

    reproduciendo = false;
}

void Reproductor::siguienteCancion() {
    if (!reproduciendo || totalCanciones == 0) return;

    avanzarCancion();
    cancionActual->incrementarReproducciones();
    contadorCancionesReproducidas++;

    mostrarInterfazReproduccion();
    mostrarPublicidad();
}

void Reproductor::cancionAnterior() {
    if (!reproduciendo || !usuarioActual->esPremium()) return;

    if (indiceHistorial > 0) {
        indiceHistorial--;
        int historialIndex = (indiceHistorial - 1) % 4;
        if (historial[historialIndex] != nullptr) {
            cancionActual = historial[historialIndex];
            cancionActual->incrementarReproducciones();
            mostrarInterfazReproduccion();
        }
    } else {
        std::cout << "No hay canciones anteriores en el historial." << std::endl;
    }
}

void Reproductor::avanzarCancion() {
    if (modoRepetir) return;

    indiceActual = (indiceActual + 1) % totalCanciones;
    cancionActual = todasLasCanciones[indiceActual];

    if (usuarioActual->esPremium()) {
        historial[indiceHistorial % 4] = cancionActual;
        indiceHistorial++;
    }
}

void Reproductor::retrocederCancion() {
    if (indiceHistorial > 0) {
        indiceHistorial--;
        int historialIndex = (indiceHistorial - 1) % 4;
        if (historial[historialIndex] != nullptr) {
            cancionActual = historial[historialIndex];
        }
    }
}

void Reproductor::detenerReproduccion() {
    reproduciendo = false;
    std::cout << "Reproducción detenida." << std::endl;
}

void Reproductor::toggleRepetir() {
    if (!usuarioActual->esPremium()) {
        std::cout << "Esta función solo está disponible para usuarios premium." << std::endl;
        return;
    }

    modoRepetir = !modoRepetir;
    std::cout << "Modo repetir " << (modoRepetir ? "activado" : "desactivado") << std::endl;
}
