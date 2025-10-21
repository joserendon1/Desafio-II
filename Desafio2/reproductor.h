#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include "Cancion.h"
#include "Usuario.h"
#include "MensajePublicitario.h"
#include <chrono>
#include <random>

class Reproductor {
private:
    Cancion** todasLasCanciones;
    MensajePublicitario** todosLosMensajes;
    int totalCanciones;
    int totalMensajes;
    Usuario* usuarioActual;

    Cancion* cancionActual;
    int indiceActual;
    bool reproduciendo;
    bool modoRepetir;
    Cancion* historial[4];
    int indiceHistorial;
    int contadorCancionesReproducidas;

    std::mt19937 generadorAleatorio;

    void mostrarInterfazReproduccion();
    void mostrarPublicidad();
    void avanzarCancion();
    void retrocederCancion();
    MensajePublicitario* obtenerMensajeAleatorio();
    int generarNumeroAleatorio(int maximo);

public:
    Reproductor(Cancion** canciones, int totalCanc, MensajePublicitario** mensajes, int totalMsg, Usuario* usuario);

    void reproducirAleatorio();
    void siguienteCancion();
    void cancionAnterior();
    void detenerReproduccion();
    void toggleRepetir();

    bool estaReproduciendo() const { return reproduciendo; }
    Cancion* getCancionActual() const { return cancionActual; }
};

#endif
