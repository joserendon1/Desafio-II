#include "Reproductor.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include "Usuario.h"
#include "MensajePublicitario.h"
#include <iostream>
#include <chrono>
#include <limits>

Reproductor::Reproductor(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario)
    : cantidadHistorial(0),
    indiceUltima(-1),
    todasLasCanciones(canciones),
    todosLosMensajes(mensajes),
    totalCanciones(totalCanc),
    totalMensajes(totalMsg),
    usuarioActual(usuario),
    cancionActual(nullptr),
    indiceActual(-1),
    reproduciendo(false),
    modoRepetir(false),
    contadorCancionesReproducidas(0),
    reproduccionListaFavoritos(false),
    iteraciones(0)
{
    for (int i = 0; i < TAMANO_HISTORIAL; i++) {
        historial[i] = nullptr;
    }

    auto ahora = std::chrono::steady_clock::now();
    auto semilla = ahora.time_since_epoch().count();
    generadorAleatorio.seed(static_cast<unsigned int>(semilla));
}

int Reproductor::generarNumeroAleatorio(int maximo) {
    //resetIteraciones();

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
    //resetIteraciones();

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
    //resetIteraciones();

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

void Reproductor::agregarAlHistorial(Cancion* cancion) {
    if (!usuarioActual->esPremium() || cancion == nullptr) return;

    if (cantidadHistorial > 0 && historial[indiceUltima] == cancion) {
        return;
    }

    if (cantidadHistorial == TAMANO_HISTORIAL) {
        for (int i = 0; i < TAMANO_HISTORIAL - 1; i++) {
            historial[i] = historial[i + 1];
        }
        historial[TAMANO_HISTORIAL - 1] = cancion;
        indiceUltima = TAMANO_HISTORIAL - 1;
    } else {
        historial[cantidadHistorial] = cancion;
        indiceUltima = cantidadHistorial;
        cantidadHistorial++;
    }

    std::cout << "Agregada al historial: " << cancion->getNombre()
              << " en posicion " << indiceUltima
              << " (total en historial: " << cantidadHistorial << "/" << TAMANO_HISTORIAL << ")" << std::endl;
}

Cancion* Reproductor::obtenerCancionAnterior() {
    if (cantidadHistorial == 0 || indiceUltima <= 0) {
        return nullptr;
    }
    return historial[indiceUltima - 1];
}

bool Reproductor::retrocederEnHistorial() {
    if (cantidadHistorial == 0 || indiceUltima <= 0) {
        return false;
    }
    indiceUltima--;

    std::cout << "Retrocediendo. Nueva ultima: " << indiceUltima << std::endl;
    return true;
}

void Reproductor::actualizarDatos(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario) {
    //resetIteraciones();

    this->todasLasCanciones = canciones;
    this->totalCanciones = totalCanc;
    this->todosLosMensajes = mensajes;
    this->totalMensajes = totalMsg;
    this->usuarioActual = usuario;

    this->cancionActual = nullptr;
    this->indiceActual = -1;
    this->reproduciendo = false;
    this->modoRepetir = false;
    this->cantidadHistorial = 0;
    this->indiceUltima = -1;
    this->contadorCancionesReproducidas = 0;

    for (int i = 0; i < TAMANO_HISTORIAL; i++) {
        this->historial[i] = nullptr;
        incrementarIteraciones();
    }
    incrementarIteraciones(15);
}

void Reproductor::reproducirAleatorio() {
    //resetIteraciones();

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

    int cancionesReproducidas = 0;
    bool esCancionNueva = true;

    while (reproduciendo && cancionesReproducidas < LIMITE_CANCIONES_PRUEBA) {
        incrementarIteraciones();

        if (esCancionNueva) {
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

            if (usuarioActual->esPremium()) {
                agregarAlHistorial(cancionActual);
            }
        }

        mostrarInterfazReproduccion();
        incrementarIteraciones(getIteraciones());

        int opcion;
        std::cin >> opcion;
        incrementarIteraciones();

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
            cancionesReproducidas++;

            std::cout << "Progreso: " << cancionesReproducidas << "/" << LIMITE_CANCIONES_PRUEBA << " canciones" << std::endl;

            if (usuarioActual->esPremium() && esCancionNueva) {
                agregarAlHistorial(cancionActual);
            }
            incrementarIteraciones(5);
            break;

        case 2:
            reproduciendo = false;
            std::cout << "Reproduccion detenida." << std::endl;
            incrementarIteraciones(2);
            break;

        case 3:
            if (usuarioActual->esPremium()) {
                std::cout << "Saltando a siguiente cancion..." << std::endl;
                esCancionNueva = true;
                incrementarIteraciones();
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                incrementarIteraciones(2);
            }
            break;

        case 4:
            if (usuarioActual->esPremium()) {
                if (cantidadHistorial > 1 && indiceUltima > 0) {
                    Cancion* cancionAnterior = obtenerCancionAnterior();
                    if (cancionAnterior != nullptr) {
                        cancionActual = cancionAnterior;
                        retrocederEnHistorial();

                        std::cout << ">>> Retrocediendo a cancion anterior: " << cancionActual->getNombre() << std::endl;

                        esCancionNueva = false;

                    } else {
                        std::cout << "No hay canciones anteriores disponibles." << std::endl;
                        esCancionNueva = true;
                    }
                } else {
                    std::cout << "No hay canciones anteriores disponibles." << std::endl;
                    esCancionNueva = true;
                }
                incrementarIteraciones(5);
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                incrementarIteraciones(2);
            }
            break;

        case 5:
            if (usuarioActual->esPremium()) {
                modoRepetir = !modoRepetir;
                std::cout << "Modo repetir " << (modoRepetir ? "activado" : "desactivado") << std::endl;
                if (modoRepetir && esCancionNueva) {
                    cancionesReproducidas--;
                }
                incrementarIteraciones(2);
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
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
            cancionesReproducidas++;
            cancionReproducida = true;

            if (usuarioActual->esPremium() && esCancionNueva) {
                agregarAlHistorial(cancionActual);
            }

            std::cout << "Progreso: " << cancionesReproducidas << "/" << LIMITE_CANCIONES_PRUEBA << " canciones" << std::endl;
            incrementarIteraciones(5);
            break;
        }

        if (modoRepetir && reproduciendo && cancionReproducida && esCancionNueva) {
            cancionesReproducidas--;
            std::cout << "Modo repetir activado - repitiendo cancion..." << std::endl;
            std::cout << "Progreso: " << cancionesReproducidas << "/" << LIMITE_CANCIONES_PRUEBA << " canciones" << std::endl;
            incrementarIteraciones(2);
        }

        if (!usuarioActual->esPremium() && contadorCancionesReproducidas > 0 &&
            contadorCancionesReproducidas % 2 == 0 && esCancionNueva && cancionReproducida) {

            MensajePublicitario* mensaje = obtenerMensajeAleatorio();
            if (mensaje != nullptr) {
                std::cout << "\n--- PUBLICIDAD ---" << std::endl;
                std::cout << mensaje->texto << std::endl;
                std::cout << "Categoría: " << mensaje->categoria << std::endl;
                std::cout << "------------------" << std::endl;

                // Pausa breve para que el usuario pueda leer la publicidad
                std::cout << "Presione Enter para continuar...";
                std::cin.get();
            }
        }

        if (opcion == 4 && !cancionReproducida) {
        } else if (cancionReproducida) {
            esCancionNueva = true;
        } else {
            esCancionNueva = true;
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

void Reproductor::reproducirListaFavoritos(Cancion** cancionesLista, int totalCancionesLista, bool ordenAleatorio) {
    //resetIteraciones();

    if (totalCancionesLista == 0) {
        std::cout << "La lista de favoritos esta vacia." << std::endl;
        incrementarIteraciones();
        return;
    }

    reproduccionListaFavoritos = true;
    this->todasLasCanciones = cancionesLista;
    this->totalCanciones = totalCancionesLista;

    reproduciendo = true;
    contadorCancionesReproducidas = 0;
    const int LIMITE_CANCIONES_PRUEBA = 5;

    std::cout << "\n==========================================" << std::endl;
    std::cout << "    REPRODUCIENDO LISTA DE FAVORITOS" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Modo: " << (ordenAleatorio ? "Aleatorio" : "Orden original") << std::endl;
    std::cout << "Total canciones: " << totalCancionesLista << std::endl;
    std::cout << "Historial disponible: " << TAMANO_HISTORIAL << " canciones" << std::endl;
    std::cout << "==========================================" << std::endl;

    int cancionesReproducidas = 0;
    bool esCancionNueva = true;
    indiceActual = -1;

    while (reproduciendo && cancionesReproducidas < LIMITE_CANCIONES_PRUEBA) {
        incrementarIteraciones();

        if (esCancionNueva) {
            if (ordenAleatorio) {
                indiceActual = generarNumeroAleatorio(totalCancionesLista);
                incrementarIteraciones(getIteraciones());
            } else {
                indiceActual = (indiceActual + 1) % totalCancionesLista;
                incrementarIteraciones();
            }

            if (indiceActual < 0 || indiceActual >= totalCancionesLista) {
                std::cout << "ERROR: Indice de canción invalido: " << indiceActual << std::endl;
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

            if (usuarioActual->esPremium()) {
                agregarAlHistorial(cancionActual);
            }
        }

        mostrarInterfazReproduccion();
        incrementarIteraciones(getIteraciones());

        int opcion;
        std::cin >> opcion;
        incrementarIteraciones();

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        bool cancionReproducida = false;

        switch (opcion) {
        case 1:
        {
            std::cout << "Reproduciendo: " << cancionActual->getNombre() << std::endl;
            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
            }
            std::cout << "Cancion finalizada." << std::endl;

            cancionActual->incrementarReproducciones();
            contadorCancionesReproducidas++;
            cancionReproducida = true;
            cancionesReproducidas++;

            std::cout << "Progreso: " << cancionesReproducidas << "/" << LIMITE_CANCIONES_PRUEBA << " canciones" << std::endl;

            if (usuarioActual->esPremium() && esCancionNueva) {
                agregarAlHistorial(cancionActual);
            }
            incrementarIteraciones(5);
        }
        break;

        case 2:
            reproduciendo = false;
            std::cout << "Reproduccion detenida." << std::endl;
            incrementarIteraciones(2);
            break;

        case 3:
            if (usuarioActual->esPremium()) {
                std::cout << "Saltando a siguiente cancion..." << std::endl;
                esCancionNueva = true;
                incrementarIteraciones();
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                incrementarIteraciones(2);
            }
            break;

        case 4:
            if (usuarioActual->esPremium()) {
                if (cantidadHistorial > 1 && indiceUltima > 0) {
                    Cancion* cancionAnterior = obtenerCancionAnterior();
                    if (cancionAnterior != nullptr) {
                        cancionActual = cancionAnterior;
                        retrocederEnHistorial();

                        std::cout << ">>> Retrocediendo a cancion anterior: " << cancionActual->getNombre() << std::endl;

                        for (int i = 0; i < totalCancionesLista; i++) {
                            if (todasLasCanciones[i] == cancionActual) {
                                indiceActual = i;
                                break;
                            }
                        }

                        esCancionNueva = false;
                        cancionesReproducidas--;

                    } else {
                        std::cout << "No hay canciones anteriores disponibles." << std::endl;
                        esCancionNueva = true;
                    }
                } else {
                    std::cout << "No hay canciones anteriores disponibles." << std::endl;
                    esCancionNueva = true;
                }
                incrementarIteraciones(5);
            } else {
                std::cout << "Opcion no disponible para usuarios estandar." << std::endl;
                incrementarIteraciones(2);
            }
            break;

        case 5:
            if (usuarioActual->esPremium()) {
                modoRepetir = !modoRepetir;
                std::cout << "Modo repetir " << (modoRepetir ? "activado" : "desactivado") << std::endl;
                if (modoRepetir && esCancionNueva) {
                    cancionesReproducidas--;
                }
                incrementarIteraciones(2);
            } else {
                std::cout << "Opcion no disponible para usuarios estándar." << std::endl;
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
            cancionesReproducidas++;
            cancionReproducida = true;

            if (usuarioActual->esPremium() && esCancionNueva) {
                agregarAlHistorial(cancionActual);
            }

            std::cout << "Progreso: " << cancionesReproducidas << "/" << LIMITE_CANCIONES_PRUEBA << " canciones" << std::endl;
            incrementarIteraciones(5);
            break;
        }

        if (modoRepetir && reproduciendo && cancionReproducida && esCancionNueva) {
            cancionesReproducidas--;
            std::cout << "Modo repetir activado - repitiendo canción..." << std::endl;
            std::cout << "Progreso: " << cancionesReproducidas << "/" << LIMITE_CANCIONES_PRUEBA << " canciones" << std::endl;
            incrementarIteraciones(2);
        }

        if (opcion == 4 && !cancionReproducida) {
        } else if (cancionReproducida) {
            esCancionNueva = true;
        } else {
            esCancionNueva = true;
        }

        incrementarIteraciones();
    }

    if (reproduciendo) {
        std::cout << "Reproduccion finalizada (limite de " << LIMITE_CANCIONES_PRUEBA << " canciones alcanzado)." << std::endl;
        incrementarIteraciones();
    }

    reproduciendo = false;
    reproduccionListaFavoritos = false; // Resetear modo
    incrementarIteraciones();
}

void Reproductor::siguienteCancion() {
    //resetIteraciones();

    if (!reproduciendo || totalCanciones == 0) {
        incrementarIteraciones();
        return;
    }

    if (!modoRepetir) {
        indiceActual = (indiceActual + 1) % totalCanciones;
        cancionActual = todasLasCanciones[indiceActual];
        incrementarIteraciones(2);

        if (usuarioActual->esPremium()) {
            agregarAlHistorial(cancionActual);
        }
    }

    cancionActual->incrementarReproducciones();
    contadorCancionesReproducidas++;
    incrementarIteraciones(2);

    mostrarInterfazReproduccion();
    incrementarIteraciones(getIteraciones());
}

void Reproductor::cancionAnterior() {
    //resetIteraciones();

    if (!reproduciendo) {
        std::cout << "No hay reproduccion en curso." << std::endl;
        incrementarIteraciones();
        return;
    }

    if (!usuarioActual->esPremium()) {
        std::cout << "Esta funcion solo esta disponible para usuarios premium." << std::endl;
        incrementarIteraciones();
        return;
    }

    if (cantidadHistorial > 1 && indiceUltima > 0) {
        Cancion* cancionAnterior = obtenerCancionAnterior();
        if (cancionAnterior != nullptr) {
            cancionActual = cancionAnterior;
            retrocederEnHistorial();
            std::cout << ">>> Retrocediendo a cancion anterior: " << cancionActual->getNombre() << std::endl;
            cancionActual->incrementarReproducciones();
        } else {
            std::cout << "No hay canciones anteriores disponibles." << std::endl;
        }
    } else {
        std::cout << "No hay canciones anteriores disponibles." << std::endl;
    }

    incrementarIteraciones(10);
}

void Reproductor::detenerReproduccion() {
    //resetIteraciones();

    reproduciendo = false;
    std::cout << "Reproduccion detenida." << std::endl;
    incrementarIteraciones(2);
}

void Reproductor::toggleRepetir() {
    //resetIteraciones();

    if (!usuarioActual->esPremium()) {
        std::cout << "Esta funcion solo esta disponible para usuarios premium." << std::endl;
        incrementarIteraciones();
        return;
    }

    modoRepetir = !modoRepetir;
    std::cout << "Modo repetir " << (modoRepetir ? "activado" : "desactivado") << std::endl;
    incrementarIteraciones(2);
}
