#include "Reproductor.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include "Usuario.h"
#include "MensajePublicitario.h"
#include <iostream>
#include <chrono>

Reproductor::Reproductor(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario)
    : todasLasCanciones(canciones),
    todosLosMensajes(mensajes),
    totalCanciones(totalCanc),
    totalMensajes(totalMsg),
    usuarioActual(usuario),
    cancionActual(nullptr),
    indiceActual(-1),
    reproduciendo(false),
    modoRepetir(false),
    indiceHistorial(0),
    contadorCancionesReproducidas(0)
{
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

    if (!usuarioActual->esPremium() && contadorCancionesReproducidas % 2 == 0 && contadorCancionesReproducidas > 0) {
        MensajePublicitario* mensaje = obtenerMensajeAleatorio();
        if (mensaje != nullptr) {
            std::cout << "\n==========================================" << std::endl;
            std::cout << "\"Mensaje publicitario\"" << std::endl;
            std::cout << "Categoria del mensaje: ";

            if (mensaje->categoria == 'A') {
                std::cout << "AAA";
            } else {
                std::cout << mensaje->categoria;
            }
            std::cout << std::endl;
            std::cout << "==========================================" << std::endl;

            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {}
        }
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "Cantante: " << cancionActual->getAlbum()->artista->nombre << std::endl;
    std::cout << "Album: " << cancionActual->getAlbum()->nombre << std::endl;
    std::cout << "Ruta a la portada del album: " << cancionActual->getAlbum()->portada << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Titulo de la cancion reproducida: " << cancionActual->getNombre() << std::endl;
    std::cout << "Ruta al archivo de audio: " << cancionActual->obtenerRuta(usuarioActual->esPremium()) << std::endl;
    std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    std::cout << "Opciones de reproduccion:" << std::endl;
    std::cout << "1.- Reproducir  2.- Detener";

    if (usuarioActual->esPremium()) {
        std::cout << "  3.- Siguiente  4.- Anterior  5.- " << (modoRepetir ? "Desactivar" : "Activar") << " repetir";
    }

    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
}

MensajePublicitario* Reproductor::obtenerMensajeAleatorio() {
    if (totalMensajes == 0) return nullptr;

    static int ultimoIndice = -1;

    if (totalMensajes == 1) {
        return todosLosMensajes[0];
    }

    int pesoTotal = 0;
    for (int i = 0; i < totalMensajes; i++) {
        pesoTotal += todosLosMensajes[i]->prioridad;
    }

    int mensajeIndex;
    do {
        int randomValue = generarNumeroAleatorio(pesoTotal);
        int accumulatedWeight = 0;
        mensajeIndex = 0;

        for (int i = 0; i < totalMensajes; i++) {
            accumulatedWeight += todosLosMensajes[i]->prioridad;
            if (randomValue < accumulatedWeight) {
                mensajeIndex = i;
                break;
            }
        }
    } while (mensajeIndex == ultimoIndice && totalMensajes > 1);

    ultimoIndice = mensajeIndex;
    return todosLosMensajes[mensajeIndex];
}

void Reproductor::actualizarDatos(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario) {
    this->todasLasCanciones = canciones;
    this->totalCanciones = totalCanc;
    this->todosLosMensajes = mensajes;
    this->totalMensajes = totalMsg;
    this->usuarioActual = usuario;

    this->cancionActual = nullptr;
    this->indiceActual = -1;
    this->reproduciendo = false;
    this->modoRepetir = false;
    this->indiceHistorial = 0;
    this->contadorCancionesReproducidas = 0;

    for (int i = 0; i < 4; i++) {
        this->historial[i] = nullptr;
    }
}

void Reproductor::reproducirAleatorio() {
    if (totalCanciones == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        return;
    }

    if (todasLasCanciones == nullptr) {
        std::cout << "ERROR: Array de canciones es nulo" << std::endl;
        return;
    }

    reproduciendo = true;
    contadorCancionesReproducidas = 0;
    const int LIMITE_CANCIONES_PRUEBA = 5;

    std::cout << "Iniciando reproduccion aleatoria..." << std::endl;
    std::cout << "Se reproduciran " << LIMITE_CANCIONES_PRUEBA << " canciones (modo prueba)" << std::endl;

    for (int i = 0; i < LIMITE_CANCIONES_PRUEBA && reproduciendo; i++) {
        indiceActual = generarNumeroAleatorio(totalCanciones);

        if (indiceActual < 0 || indiceActual >= totalCanciones) {
            std::cout << "ERROR: Indice de cancion invalido: " << indiceActual << std::endl;
            reproduciendo = false;
            break;
        }

        cancionActual = todasLasCanciones[indiceActual];

        if (cancionActual == nullptr) {
            std::cout << "ERROR: Cancion actual es nula en indice " << indiceActual << std::endl;
            reproduciendo = false;
            break;
        }

        if (cancionActual->getAlbum() == nullptr) {
            std::cout << "ERROR: Cancion sin album: " << cancionActual->getNombre() << std::endl;
            reproduciendo = false;
            break;
        }

        if (cancionActual->getAlbum()->artista == nullptr) {
            std::cout << "ERROR: Album sin artista: " << cancionActual->getAlbum()->nombre << std::endl;
            reproduciendo = false;
            break;
        }

        if (usuarioActual->esPremium()) {
            historial[indiceHistorial % 4] = cancionActual;
            indiceHistorial++;
        }

        cancionActual->incrementarReproducciones();
        contadorCancionesReproducidas++;

        mostrarInterfazReproduccion();

        int opcion;
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            char c;
            while (std::cin.get(c) && c != '\n') {}
            std::cout << "Entrada invalida. Continuando..." << std::endl;
            opcion = 1;
        }

        switch (opcion) {
        case 1:
            std::cout << "Reproduciendo..." << std::endl;
            {
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
                }
            }
            std::cout << "Cancion finalizada." << std::endl;
            break;

        case 2:
            if (reproduciendo) {
                reproduciendo = false;
                std::cout << "Reproduccion detenida." << std::endl;
            } else {
                std::cout << "No hay reproduccion en curso para detener." << std::endl;
                i--;
            }
            break;

        case 3:
            if (usuarioActual->esPremium()) {
                if (reproduciendo) {
                    std::cout << "Saltando a siguiente cancion..." << std::endl;
                } else {
                    std::cout << "No hay reproduccion en curso." << std::endl;
                    i--;
                }
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
            }
            break;

        case 4:
            if (usuarioActual->esPremium()) {
                if (reproduciendo && indiceHistorial > 1) {
                    cancionAnterior();
                    contadorCancionesReproducidas--;
                    i--;
                } else {
                    std::cout << "No hay canciones anteriores disponibles." << std::endl;
                    i--;
                }
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
            }
            break;

        case 5:
            if (usuarioActual->esPremium()) {
                if (reproduciendo) {
                    toggleRepetir();
                    i--;
                } else {
                    std::cout << "No hay reproduccion en curso." << std::endl;
                    i--;
                }
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
        std::cout << "Reproduccion finalizada (limite de " << LIMITE_CANCIONES_PRUEBA << " canciones alcanzado)." << std::endl;
    }

    reproduciendo = false;
}

void Reproductor::siguienteCancion() {
    if (!reproduciendo || totalCanciones == 0) return;

    if (!modoRepetir) {
        indiceActual = (indiceActual + 1) % totalCanciones;
        cancionActual = todasLasCanciones[indiceActual];

        if (usuarioActual->esPremium()) {
            historial[indiceHistorial % 4] = cancionActual;
            indiceHistorial++;
        }
    }

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
