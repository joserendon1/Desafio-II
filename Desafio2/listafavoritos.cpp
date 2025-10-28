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
    //resetIteraciones();

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
    //resetIteraciones();

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
    //resetIteraciones();

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

    incrementarIteraciones(iteracionesLocales + 1);
    return array;
}

int ListaFavoritos::getTotalCancionesSeguidas() const {
    //resetIteraciones();
    if (listaSeguida != nullptr) {
        incrementarIteraciones();
        return listaSeguida->getTotalCanciones();
    }
    incrementarIteraciones();
    return 0;
}

int ListaFavoritos::getTotalCancionesVisibles() const {
    //resetIteraciones();
    int total = getTotalCanciones() + getTotalCancionesSeguidas();
    incrementarIteraciones();
    return total;
}

Cancion** ListaFavoritos::getCancionesConSeguidas(bool ordenAleatorio) const {
    //resetIteraciones();

    int totalVisibles = getTotalCancionesVisibles();
    if (totalVisibles == 0) {
        incrementarIteraciones();
        return nullptr;
    }

    Cancion** arrayCompleto = new Cancion*[totalVisibles];
    int currentIndex = 0;
    int iteracionesLocales = 0;

    ContenedorCancionFavorita* actual = inicio;
    while (actual != nullptr) {
        iteracionesLocales++;
        arrayCompleto[currentIndex++] = actual->contenido;
        actual = actual->siguiente;
    }

    if (listaSeguida != nullptr) {
        Cancion** cancionesSeguidas = listaSeguida->getCancionesArray();
        if (cancionesSeguidas != nullptr) {
            for (int i = 0; i < listaSeguida->getTotalCanciones(); i++) {
                iteracionesLocales++;
                arrayCompleto[currentIndex++] = cancionesSeguidas[i];
            }
            delete[] cancionesSeguidas;
            iteracionesLocales++;
        }
    }

    if (ordenAleatorio) {
        auto semilla = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generador(semilla);

        for (int i = totalVisibles - 1; i > 0; --i) {
            std::uniform_int_distribution<int> distribucion(0, i);
            int j = distribucion(generador);
            Cancion* temp = arrayCompleto[i];
            arrayCompleto[i] = arrayCompleto[j];
            arrayCompleto[j] = temp;
            iteracionesLocales++;
        }
    }

    incrementarIteraciones(iteracionesLocales + 1);
    return arrayCompleto;
}

void ListaFavoritos::seguirLista(ListaFavoritos* otraLista) {
    //resetIteraciones();

    if (listaSeguida != nullptr) {
        std::cout << "Ya estas siguiendo una lista. Deja de seguir primero." << std::endl;
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
   // resetIteraciones();

    if (listaSeguida == nullptr) {
        std::cout << "No estas siguiendo ninguna lista." << std::endl;
        incrementarIteraciones();
        return;
    }

    std::cout << "Dejaste de seguir la lista de: " << listaSeguida->getUsuario()->getNickname() << std::endl;
    listaSeguida = nullptr;
    incrementarIteraciones(2);
}

void ListaFavoritos::mostrarLista() const {
    //resetIteraciones();

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
