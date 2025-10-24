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

    if (!usuarioActual->esPremium() && contadorCancionesReproducidas % 2 == 0 && totalMensajes > 0) {
        MensajePublicitario* mensaje = obtenerMensajeAleatorio();
        if (mensaje != nullptr) {
            std::cout << "\n--- MENSAJE PUBLICITARIO ---" << std::endl;
            std::cout << "Categoria: ";

            if (mensaje->categoria == 'A') {
                std::cout << "AAA";
            } else if (mensaje->categoria == 'B') {
                std::cout << "B";
            } else {
                std::cout << "C";
            }
            std::cout << std::endl;

            std::cout << "\"" << mensaje->texto << "\"" << std::endl;
            std::cout << "Mostrando publicidad..." << std::endl;

            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {}
        }
    }

    std::cout << "\n--- REPRODUCIENDO ---" << std::endl;
    std::cout << "Artista: " << cancionActual->getAlbum()->artista->nombre << std::endl;
    std::cout << "Album: " << cancionActual->getAlbum()->nombre << std::endl;
    std::cout << "Portada: " << cancionActual->getAlbum()->portada << std::endl;
    std::cout << "Cancion: " << cancionActual->getNombre() << std::endl;
    std::cout << "Audio: " << cancionActual->obtenerRuta(usuarioActual->esPremium()) << std::endl;
    std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;
    std::cout << "---------------------" << std::endl;

    std::cout << "Opciones:" << std::endl;
    std::cout << "1. Reproducir  2. Detener";

    if (usuarioActual->esPremium()) {
        std::cout << "  3. Siguiente  4. Anterior  5. " << (modoRepetir ? "Desactivar" : "Activar") << " repetir";
    }

    std::cout << std::endl;
    std::cout << "Seleccione una opcion: ";
}

MensajePublicitario* Reproductor::obtenerMensajeAleatorio() {
    if (totalMensajes == 0) return nullptr;

    int pesoTotal = 0;
    for (int i = 0; i < totalMensajes; i++) {
        pesoTotal += todosLosMensajes[i]->prioridad;
    }

    int randomValue = generarNumeroAleatorio(pesoTotal);
    int accumulatedWeight = 0;

    for (int i = 0; i < totalMensajes; i++) {
        accumulatedWeight += todosLosMensajes[i]->prioridad;
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

    std::cout << "Iniciando reproduccion aleatoria..." << std::endl;
    std::cout << "Se reproduciran 5 canciones (modo prueba)" << std::endl;

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

        int opcion;
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            std::cout << "Reproduciendo..." << std::endl;
            {
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {}
            }
            std::cout << "Cancion finalizada." << std::endl;
            break;

        case 2:
            reproduciendo = false;
            std::cout << "Reproduccion detenida." << std::endl;
            break;

        case 3:
            if (usuarioActual->esPremium()) {
                std::cout << "Saltando a siguiente cancion..." << std::endl;
                i--;
                siguienteCancion();
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
            }
            break;

        case 4:
            if (usuarioActual->esPremium()) {
                cancionAnterior();
                i--;
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
            }
            break;

        case 5:
            if (usuarioActual->esPremium()) {
                toggleRepetir();
                i--;
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
            }
            break;

        default:
            std::cout << "Opcion no valida. Reproduciendo automaticamente..." << std::endl;
            {
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {}
            }
            std::cout << "Cancion finalizada." << std::endl;
            break;
        }

        if (modoRepetir && reproduciendo) {
            i--;
            std::cout << "Modo repetir activado - repitiendo cancion..." << std::endl;
        }
    }

    if (reproduciendo) {
        std::cout << "Reproduccion finalizada (limite de 5 canciones alcanzado)." << std::endl;
    }

    reproduciendo = false;
}

void Reproductor::siguienteCancion() {
    if (!reproduciendo || totalCanciones == 0) return;

    avanzarCancion();
    cancionActual->incrementarReproducciones();
    contadorCancionesReproducidas++;

    mostrarInterfazReproduccion();
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
    std::cout << "Reproduccion detenida." << std::endl;
}

void Reproductor::toggleRepetir() {
    if (!usuarioActual->esPremium()) {
        std::cout << "Esta funcion solo esta disponible para usuarios premium." << std::endl;
        return;
    }

    modoRepetir = !modoRepetir;
    std::cout << "Modo repetir " << (modoRepetir ? "activado" : "desactivado") << std::endl;
}
