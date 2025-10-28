#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include "Cancion.h"
#include "Usuario.h"
#include "MensajePublicitario.h"
#include <chrono>
#include <random>

class Reproductor {
private:
    static const int TAMANO_HISTORIAL = 6;

    Cancion* historial[TAMANO_HISTORIAL];
    int cantidadHistorial;
    int indiceUltima;

    Cancion** todasLasCanciones;
    MensajePublicitario** todosLosMensajes;
    int totalCanciones;
    int totalMensajes;
    Usuario* usuarioActual;

    Cancion* cancionActual;
    int indiceActual;
    bool reproduciendo;
    bool modoRepetir;

    int contadorCancionesReproducidas;
    bool reproduccionListaFavoritos;

    std::mt19937 generadorAleatorio;
    mutable unsigned long iteraciones;

    void mostrarInterfazReproduccion();
    MensajePublicitario* obtenerMensajeAleatorio();
    int generarNumeroAleatorio(int maximo);

    void agregarAlHistorial(Cancion* cancion);
    Cancion* obtenerCancionAnterior();
    bool retrocederEnHistorial();

public:
    Reproductor(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario);

    void actualizarDatos(Cancion** canciones, int totalCanc,
                         MensajePublicitario** mensajes, int totalMsg,
                         Usuario* usuario);

    void reproducirAleatorio();
    void reproducirListaFavoritos(Cancion** cancionesLista, int totalCancionesLista, bool ordenAleatorio); // Nuevo método
    void siguienteCancion();
    void cancionAnterior();
    void detenerReproduccion();
    void toggleRepetir();

    unsigned long getIteraciones() const { return iteraciones; }
    void resetIteraciones() const { iteraciones = 0; }
    void incrementarIteraciones(int cantidad = 1) const { iteraciones += cantidad; }

    bool estaReproduciendo() const { return reproduciendo; }
    Cancion* getCancionActual() const { return cancionActual; }
};

#endif
