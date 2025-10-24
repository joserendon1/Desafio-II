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
    contadorCancionesReproducidas(0),
    iteraciones(0)
{
    for (int i = 0; i < 4; i++) {
        historial[i] = nullptr;
        incrementarIteraciones();
    }

    auto ahora = std::chrono::steady_clock::now();
    auto semilla = ahora.time_since_epoch().count();
    generadorAleatorio.seed(static_cast<unsigned int>(semilla));
    incrementarIteraciones(3);
}

int Reproductor::generarNumeroAleatorio(int maximo) {
    resetIteraciones();

    if (maximo <= 0) {
        incrementarIteraciones();
        return 0;
    }

    std::uniform_int_distribution<int> distribucion(0, maximo - 1);
    int resultado = distribucion(generadorAleatorio);
    incrementarIteraciones(2);

    return resultado;
}

void Reproductor::mostrarInterfazReproduccion() {
    resetIteraciones();

    if (cancionActual == nullptr) {
        incrementarIteraciones();
        return;
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
        incrementarIteraciones();
    }

    std::cout << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Seleccione una opcion: ";
    incrementarIteraciones(15);
}

MensajePublicitario* Reproductor::obtenerMensajeAleatorio() {
    resetIteraciones();

    if (totalMensajes == 0) {
        incrementarIteraciones();
        return nullptr;
    }

    static int ultimoIndice = -1;
    incrementarIteraciones();

    if (totalMensajes == 1) {
        incrementarIteraciones();
        return todosLosMensajes[0];
    }

    int pesoTotal = 0;
    for (int i = 0; i < totalMensajes; i++) {
        pesoTotal += todosLosMensajes[i]->prioridad;
        incrementarIteraciones();
    }

    int mensajeIndex;
    int intentos = 0;
    const int MAX_INTENTOS = 10;

    do {
        intentos++;
        int randomValue = generarNumeroAleatorio(pesoTotal);
        incrementarIteraciones();

        int accumulatedWeight = 0;
        mensajeIndex = 0;

        for (int i = 0; i < totalMensajes; i++) {
            incrementarIteraciones();
            accumulatedWeight += todosLosMensajes[i]->prioridad;
            if (randomValue < accumulatedWeight) {
                mensajeIndex = i;
                break;
            }
        }

        if (intentos >= MAX_INTENTOS) {
            break;
        }

    } while (mensajeIndex == ultimoIndice && totalMensajes > 1);

    ultimoIndice = mensajeIndex;
    incrementarIteraciones(intentos + 2);
    return todosLosMensajes[mensajeIndex];
}

void Reproductor::actualizarDatos(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario) {
    resetIteraciones();

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
        incrementarIteraciones();
    }
    incrementarIteraciones(15);
}

void Reproductor::reproducirAleatorio() {
    resetIteraciones();

    if (totalCanciones == 0) {
        std::cout << "No hay canciones disponibles para reproducir." << std::endl;
        incrementarIteraciones();
        return;
    }

    if (todasLasCanciones == nullptr) {
        std::cout << "ERROR: Array de canciones es nulo" << std::endl;
        incrementarIteraciones();
        return;
    }

    reproduciendo = true;
    contadorCancionesReproducidas = 0;
    const int LIMITE_CANCIONES_PRUEBA = 5;

    std::cout << "Iniciando reproduccion aleatoria..." << std::endl;
    std::cout << "Se reproduciran " << LIMITE_CANCIONES_PRUEBA << " canciones (modo prueba)" << std::endl;
    incrementarIteraciones(5);

    for (int i = 0; i < LIMITE_CANCIONES_PRUEBA && reproduciendo; i++) {
        incrementarIteraciones();

        indiceActual = generarNumeroAleatorio(totalCanciones);
        incrementarIteraciones(getIteraciones());

        if (indiceActual < 0 || indiceActual >= totalCanciones) {
            std::cout << "ERROR: Indice de cancion invalido: " << indiceActual << std::endl;
            reproduciendo = false;
            incrementarIteraciones(2);
            break;
        }

        cancionActual = todasLasCanciones[indiceActual];
        incrementarIteraciones();

        if (cancionActual == nullptr) {
            std::cout << "ERROR: Cancion actual es nula en indice " << indiceActual << std::endl;
            reproduciendo = false;
            incrementarIteraciones(2);
            break;
        }

        if (cancionActual->getAlbum() == nullptr) {
            std::cout << "ERROR: Cancion sin album: " << cancionActual->getNombre() << std::endl;
            reproduciendo = false;
            incrementarIteraciones(2);
            break;
        }

        if (cancionActual->getAlbum()->artista == nullptr) {
            std::cout << "ERROR: Album sin artista: " << cancionActual->getAlbum()->nombre << std::endl;
            reproduciendo = false;
            incrementarIteraciones(2);
            break;
        }

        if (usuarioActual->esPremium()) {
            historial[indiceHistorial % 4] = cancionActual;
            indiceHistorial++;
            incrementarIteraciones(2);
        }

        if (!usuarioActual->esPremium() && contadorCancionesReproducidas > 0 && contadorCancionesReproducidas % 2 == 0) {
            MensajePublicitario* mensaje = obtenerMensajeAleatorio();
            incrementarIteraciones();

            if (mensaje != nullptr) {
                std::cout << "\n==========================================" << std::endl;
                std::cout << "\"" << mensaje->texto << "\"" << std::endl;
                std::cout << "Categoria del mensaje: ";

                if (mensaje->categoria == 'A') {
                    std::cout << "AAA";
                } else {
                    std::cout << mensaje->categoria;
                }
                std::cout << std::endl;
                std::cout << "==========================================" << std::endl;

                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {
                }
                incrementarIteraciones(10);
            }
            incrementarIteraciones();
        }

        mostrarInterfazReproduccion();
        incrementarIteraciones(getIteraciones());

        int opcion;
        std::cin >> opcion;
        incrementarIteraciones();

        if (std::cin.fail()) {
            std::cin.clear();
            char c;
            while (std::cin.get(c) && c != '\n') {
            }
            std::cout << "Entrada invalida. Continuando..." << std::endl;
            opcion = 1;
            incrementarIteraciones(3);
        }

        bool cancionReproducida = false;

        switch (opcion) {
        case 1:
            std::cout << "Reproduciendo..." << std::endl;
            {
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
                }
            }
            std::cout << "Cancion finalizada." << std::endl;

            cancionActual->incrementarReproducciones();
            contadorCancionesReproducidas++;
            cancionReproducida = true;
            incrementarIteraciones(5);
            break;

        case 2:
            if (reproduciendo) {
                reproduciendo = false;
                std::cout << "Reproduccion detenida." << std::endl;
                incrementarIteraciones(2);
            } else {
                std::cout << "No hay reproduccion en curso para detener." << std::endl;
                i--;
                incrementarIteraciones(2);
            }
            break;

        case 3:
            if (usuarioActual->esPremium()) {
                if (reproduciendo) {
                    std::cout << "Saltando a siguiente cancion..." << std::endl;
                    incrementarIteraciones();
                } else {
                    std::cout << "No hay reproduccion en curso." << std::endl;
                    i--;
                    incrementarIteraciones(2);
                }
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
                incrementarIteraciones(2);
            }
            break;

        case 4:
            if (usuarioActual->esPremium()) {
                if (reproduciendo && indiceHistorial > 1) {
                    cancionAnterior();
                    incrementarIteraciones(getIteraciones());
                    i--;
                    incrementarIteraciones(2);
                } else {
                    std::cout << "No hay canciones anteriores disponibles." << std::endl;
                    i--;
                    incrementarIteraciones(2);
                }
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
                incrementarIteraciones(2);
            }
            break;

        case 5:
            if (usuarioActual->esPremium()) {
                if (reproduciendo) {
                    toggleRepetir();
                    incrementarIteraciones(getIteraciones());
                    i--;
                    incrementarIteraciones();
                } else {
                    std::cout << "No hay reproduccion en curso." << std::endl;
                    i--;
                    incrementarIteraciones(2);
                }
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                i--;
                incrementarIteraciones(2);
            }
            break;

        default:
            std::cout << "Opcion no valida. Reproduciendo automaticamente..." << std::endl;
            {
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
                }
            }
            std::cout << "Cancion finalizada." << std::endl;

            cancionActual->incrementarReproducciones();
            contadorCancionesReproducidas++;
            cancionReproducida = true;
            incrementarIteraciones(5);
            break;
        }

        if (modoRepetir && reproduciendo && cancionReproducida) {
            i--;
            std::cout << "Modo repetir activado - repitiendo cancion..." << std::endl;
            incrementarIteraciones(2);
        }
        incrementarIteraciones();
    }

    if (reproduciendo) {
        std::cout << "Reproduccion finalizada (limite de " << LIMITE_CANCIONES_PRUEBA << " canciones alcanzado)." << std::endl;
        incrementarIteraciones();
    }

    reproduciendo = false;
    incrementarIteraciones();
}

void Reproductor::siguienteCancion() {
    resetIteraciones();

    if (!reproduciendo || totalCanciones == 0) {
        incrementarIteraciones();
        return;
    }

    if (!modoRepetir) {
        indiceActual = (indiceActual + 1) % totalCanciones;
        cancionActual = todasLasCanciones[indiceActual];
        incrementarIteraciones(2);

        if (usuarioActual->esPremium()) {
            historial[indiceHistorial % 4] = cancionActual;
            indiceHistorial++;
            incrementarIteraciones(2);
        }
    }

    cancionActual->incrementarReproducciones();
    contadorCancionesReproducidas++;
    incrementarIteraciones(2);

    mostrarInterfazReproduccion();
    incrementarIteraciones(getIteraciones());
}

void Reproductor::cancionAnterior() {
    resetIteraciones();

    if (!reproduciendo || !usuarioActual->esPremium()) {
        incrementarIteraciones();
        return;
    }

    if (indiceHistorial > 0) {
        indiceHistorial--;
        int historialIndex = (indiceHistorial - 1) % 4;
        incrementarIteraciones(2);

        if (historial[historialIndex] != nullptr) {
            cancionActual = historial[historialIndex];
            cancionActual->incrementarReproducciones();
            mostrarInterfazReproduccion();
            incrementarIteraciones(getIteraciones() + 2);
        }
    } else {
        std::cout << "No hay canciones anteriores en el historial." << std::endl;
        incrementarIteraciones();
    }
}

void Reproductor::detenerReproduccion() {
    resetIteraciones();

    reproduciendo = false;
    std::cout << "Reproduccion detenida." << std::endl;
    incrementarIteraciones(2);
}

void Reproductor::toggleRepetir() {
    resetIteraciones();

    if (!usuarioActual->esPremium()) {
        std::cout << "Esta funcion solo esta disponible para usuarios premium." << std::endl;
        incrementarIteraciones();
        return;
    }

    modoRepetir = !modoRepetir;
    std::cout << "Modo repetir " << (modoRepetir ? "activado" : "desactivado") << std::endl;
    incrementarIteraciones(2);
}
