#include "ListaFavoritos.h"
#include "Usuario.h"
#include "Cancion.h"
#include "Album.h"
#include "Artista.h"
#include <iostream>
#include <chrono>
#include <random>

ListaFavoritos::ListaFavoritos(Usuario* usuario)
    : inicio(nullptr), totalCanciones(0), usuario(usuario), listaSeguida(nullptr), iteraciones(0) {
}

ListaFavoritos::~ListaFavoritos() {
    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        ContenedorCancionFavorita* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

bool ListaFavoritos::agregarCancion(Cancion* cancion) {
    resetIteraciones();

    if (cancion == nullptr) {
        incrementarIteraciones();
        return false;
    }

    if (contieneCancion(cancion->getId())) {
        incrementarIteraciones();
        return false;
    }

    if (totalCanciones >= 10000) {
        std::cout << "Limite alcanzado: No se pueden agregar mas de 10,000 canciones a favoritos." << std::endl;
        incrementarIteraciones();
        return false;
    }

    ContenedorCancionFavorita* nuevoContenedor = new ContenedorCancionFavorita(cancion);
    incrementarIteraciones();

    if (inicio == nullptr) {
        inicio = nuevoContenedor;
        incrementarIteraciones();
    } else {
        ContenedorCancionFavorita* actual = inicio;
        incrementarIteraciones();

        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
            incrementarIteraciones();
        }
        actual->siguiente = nuevoContenedor;
        incrementarIteraciones();
    }

    totalCanciones++;
    incrementarIteraciones();

    std::cout << "Cancion agregada a favoritos: " << cancion->getNombre() << std::endl;
    return true;
}

bool ListaFavoritos::eliminarCancion(int idCancion) {
    resetIteraciones();

    ContenedorCancionFavorita* actual = inicio;
    ContenedorCancionFavorita* anterior = nullptr;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        if (actual->contenido->getId() == idCancion) {
            if (anterior == nullptr) {
                inicio = actual->siguiente;
                incrementarIteraciones();
            } else {
                anterior->siguiente = actual->siguiente;
                incrementarIteraciones();
            }
            delete actual;
            totalCanciones--;
            incrementarIteraciones();

            incrementarIteraciones(iteracionesLocales + 1);
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return false;
}

bool ListaFavoritos::contieneCancion(int idCancion) const {
    ContenedorCancionFavorita* actual = inicio;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        if (actual->contenido->getId() == idCancion) {
            incrementarIteraciones(iteracionesLocales);
            return true;
        }
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales);
    return false;
}

Cancion** ListaFavoritos::getCancionesArray() const {
    resetIteraciones();

    if (totalCanciones == 0) {
        incrementarIteraciones();
        return nullptr;
    }

    Cancion** array = new Cancion*[totalCanciones];
    ContenedorCancionFavorita* actual = inicio;
    int index = 0;
    int iteracionesLocales = 0;

    while (actual != nullptr) {
        iteracionesLocales++;
        array[index++] = actual->contenido;
        actual = actual->siguiente;
    }

    incrementarIteraciones(iteracionesLocales + 1); // +1 por la creación del array
    return array;
}

int ListaFavoritos::getTotalCancionesSeguidas() const {
    resetIteraciones();

    if (listaSeguida != nullptr) {
        incrementarIteraciones();
        return listaSeguida->getTotalCanciones();
    }
    incrementarIteraciones();
    return 0;
}

int ListaFavoritos::getTotalCancionesVisibles() const {
    resetIteraciones();

    int total = getTotalCanciones() + getTotalCancionesSeguidas();
    incrementarIteraciones();
    return total;
}

void ListaFavoritos::reproducir(bool ordenAleatorio) const {
    int totalVisibles = getTotalCancionesVisibles();
    const int MAX_REGRESAR = 6;

    if (totalVisibles == 0) {
        std::cout << "La lista de favoritos esta vacia." << std::endl;
        return;
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "       REPRODUCIENDO LISTA DE FAVORITOS" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalVisibles << std::endl;
    std::cout << " - Propias: " << getTotalCanciones() << std::endl;
    std::cout << " - Seguidas: " << getTotalCancionesSeguidas() << std::endl;

    if (ordenAleatorio) {
        std::cout << "Modo: Aleatorio" << std::endl;
    } else {
        std::cout << "Modo: Orden original" << std::endl;
    }
    std::cout << "Puede regresar hasta " << MAX_REGRESAR << " canciones" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    Cancion** ordenReproduccion = new Cancion*[totalVisibles];
    int currentIndex = 0;

    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        ordenReproduccion[currentIndex++] = actual->contenido;
        actual = actual->siguiente;
    }

    if (listaSeguida != nullptr) {
        Cancion** cancionesSeguidas = listaSeguida->getCancionesArray();
        if (cancionesSeguidas != nullptr) {
            for (int i = 0; i < listaSeguida->getTotalCanciones(); i++) {
                ordenReproduccion[currentIndex++] = cancionesSeguidas[i];
            }
            delete[] cancionesSeguidas;
        }
    }

    if (ordenAleatorio) {
        auto semilla = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generador(semilla);

        for (int i = totalVisibles - 1; i > 0; --i) {
            std::uniform_int_distribution<int> distribucion(0, i);
            int j = distribucion(generador);
            Cancion* temp = ordenReproduccion[i];
            ordenReproduccion[i] = ordenReproduccion[j];
            ordenReproduccion[j] = temp;
        }
    }

    Cancion* historial[MAX_REGRESAR] = {nullptr};
    int historialCount = 0;
    int indiceActual = 0;
    bool reproduciendo = true;
    int contadorCancionesUsuario = 0;
    bool esUsuarioPremium = usuario->esPremium();

    while (reproduciendo && indiceActual < totalVisibles) {
        Cancion* cancionActual = ordenReproduccion[indiceActual];

        if (historialCount < MAX_REGRESAR) {
            historial[historialCount] = cancionActual;
            historialCount++;
        } else {
            for (int i = 0; i < MAX_REGRESAR - 1; i++) {
                historial[i] = historial[i + 1];
            }
            historial[MAX_REGRESAR - 1] = cancionActual;
        }

        if (!esUsuarioPremium && contadorCancionesUsuario % 2 == 0 && contadorCancionesUsuario > 0) {
            std::cout << "\n--- PUBLICIDAD ---" << std::endl;
            std::cout << "Mensaje publicitario mostrado..." << std::endl;
            std::cout << "-------------------" << std::endl;

            auto start = std::chrono::steady_clock::now();
            while (std::chrono::steady_clock::now() - start < std::chrono::seconds(2)) {}
        }

        std::cout << "\nREPRODUCIENDO [" << (indiceActual + 1) << "/" << totalVisibles << "]:" << std::endl;
        std::cout << "Artista: " << cancionActual->getAlbum()->artista->nombre << std::endl;
        std::cout << "Album: " << cancionActual->getAlbum()->nombre << std::endl;
        std::cout << "Cancion: " << cancionActual->getNombre() << std::endl;
        std::cout << "Duracion: " << cancionActual->getDuracion() << " minutos" << std::endl;

        if (indiceActual < getTotalCanciones()) {
            std::cout << "Tipo: PROPIA" << std::endl;
        } else {
            std::cout << "Tipo: SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << std::endl;
        }

        std::cout << "------------------------------------------" << std::endl;

        std::cout << "Opciones:" << std::endl;
        std::cout << "1. Reproducir cancion actual" << std::endl;
        std::cout << "2. Detener reproduccion" << std::endl;

        if (historialCount > 1) {
            std::cout << "3. Cancion anterior (puede regresar hasta " << std::min(historialCount - 1, MAX_REGRESAR) << " canciones)" << std::endl;
        }

        if (indiceActual < totalVisibles - 1) {
            std::cout << "4. Siguiente cancion" << std::endl;
        }

        std::cout << "Seleccione una opcion: ";

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
            cancionActual->incrementarReproducciones();
            contadorCancionesUsuario++;
            indiceActual++;
            break;

        case 2:
            reproduciendo = false;
            std::cout << "Reproduccion detenida." << std::endl;
            break;

        case 3:
            if (historialCount > 1) {
                std::cout << "¿Cuantas canciones desea regresar? (1-" << std::min(historialCount - 1, MAX_REGRESAR) << "): ";
                int cantidadRegresar;
                std::cin >> cantidadRegresar;

                int maxPermitido = std::min(historialCount - 1, MAX_REGRESAR);
                if (cantidadRegresar >= 1 && cantidadRegresar <= maxPermitido) {
                    indiceActual -= cantidadRegresar;
                    historialCount -= cantidadRegresar;
                    contadorCancionesUsuario -= cantidadRegresar;

                    if (indiceActual < 0) indiceActual = 0;
                    if (historialCount < 0) historialCount = 0;
                    if (contadorCancionesUsuario < 0) contadorCancionesUsuario = 0;

                    std::cout << "Regresando " << cantidadRegresar << " canciones..." << std::endl;
                    continue;
                } else {
                    std::cout << "Cantidad invalida. Debe ser entre 1 y " << maxPermitido << ". Permaneciendo en la cancion actual." << std::endl;
                }
            } else {
                std::cout << "No hay canciones anteriores en el historial." << std::endl;
            }
            indiceActual++;
            break;

        case 4:
            if (indiceActual < totalVisibles - 1) {
                std::cout << "Saltando a siguiente cancion..." << std::endl;
                indiceActual++;
            } else {
                std::cout << "No hay mas canciones en la lista." << std::endl;
            }
            break;

        default:
            std::cout << "Opcion no valida. Reproduciendo automaticamente..." << std::endl;
            {
                auto start = std::chrono::steady_clock::now();
                while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {}
            }
            std::cout << "Cancion finalizada." << std::endl;
            cancionActual->incrementarReproducciones();
            contadorCancionesUsuario++;
            indiceActual++;
            break;
        }
    }

    delete[] ordenReproduccion;

    if (indiceActual >= totalVisibles) {
        std::cout << "\n==========================================" << std::endl;
        std::cout << "Lista de favoritos reproducida completamente." << std::endl;
        std::cout << "Total de canciones reproducidas: " << contadorCancionesUsuario << std::endl;
        std::cout << "==========================================" << std::endl;
    } else {
        std::cout << "\nReproduccion interrumpida por el usuario." << std::endl;
    }
}

void ListaFavoritos::seguirLista(ListaFavoritos* otraLista) {
    resetIteraciones();

    if (listaSeguida != nullptr) {
        std::cout << "Ya estás siguiendo una lista. Deja de seguir primero." << std::endl;
        incrementarIteraciones();
        return;
    }

    if (otraLista == this) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        incrementarIteraciones(2);
        return;
    }

    if (otraLista->getUsuario() == usuario) {
        std::cout << "No puedes seguir tu propia lista." << std::endl;
        incrementarIteraciones(3);
        return;
    }

    listaSeguida = otraLista;
    incrementarIteraciones(4);
    std::cout << "Ahora sigues la lista de: " << otraLista->getUsuario()->getNickname() << std::endl;
}

void ListaFavoritos::dejarDeSeguirLista() {
    resetIteraciones();

    if (listaSeguida == nullptr) {
        std::cout << "No estás siguiendo ninguna lista." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::cout << "Dejaste de seguir la lista de: " << listaSeguida->getUsuario()->getNickname() << std::endl;
    listaSeguida = nullptr;
    incrementarIteraciones(2);
}

void ListaFavoritos::mostrarLista() const {
    resetIteraciones();

    int totalVisibles = getTotalCancionesVisibles();
    int iteracionesLocales = 0;

    if (totalVisibles == 0) {
        std::cout << "Tu lista de favoritos esta vacia." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "          LISTA DE FAVORITOS" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Total de canciones: " << totalVisibles << std::endl;
    std::cout << " - Propias: " << getTotalCanciones() << std::endl;
    std::cout << " - Seguidas: " << getTotalCancionesSeguidas() << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        iteracionesLocales++;
        std::cout << "[PROPIA] " << actual->contenido->getNombre()
                  << " | Artista: " << actual->contenido->getAlbum()->artista->nombre << std::endl;
        actual = actual->siguiente;
    }

    if (listaSeguida != nullptr) {
        iteracionesLocales++;
        Cancion** cancionesSeguidas = listaSeguida->getCancionesArray();
        if (cancionesSeguidas != nullptr) {
            for (int i = 0; i < listaSeguida->getTotalCanciones(); i++) {
                iteracionesLocales++;
                std::cout << "[SEGUIDA de " << listaSeguida->getUsuario()->getNickname() << "] "
                          << cancionesSeguidas[i]->getNombre()
                          << " | Artista: " << cancionesSeguidas[i]->getAlbum()->artista->nombre << std::endl;
            }
            delete[] cancionesSeguidas;
            iteracionesLocales++;
        }
    }

    std::cout << "==========================================" << std::endl;
    incrementarIteraciones(iteracionesLocales);
}

unsigned long ListaFavoritos::calcularMemoria() const {
    unsigned long memoria = sizeof(ListaFavoritos);
    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        memoria += sizeof(ContenedorCancionFavorita);
        actual = actual->siguiente;
    }
    return memoria;
}
