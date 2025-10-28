#ifndef GESTORCATALOGO_H
#define GESTORCATALOGO_H

#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include <string>

class GestorCatalogo {
private:
    struct ContenedorArtista {
        Artista* contenido;
        ContenedorArtista* siguiente;
        ContenedorArtista(Artista* a) : contenido(a), siguiente(nullptr) {}
    };

    struct ContenedorAlbum {
        Album* contenido;
        ContenedorAlbum* siguiente;
        ContenedorAlbum(Album* a) : contenido(a), siguiente(nullptr) {}
    };

    struct ContenedorCancion {
        Cancion* contenido;
        ContenedorCancion* siguiente;
        ContenedorCancion(Cancion* c) : contenido(c), siguiente(nullptr) {}
    };

    ContenedorArtista* inicioArtistas;
    ContenedorAlbum* inicioAlbumes;
    ContenedorCancion* inicioCanciones;

    int totalArtistas;
    int totalAlbumes;
    int totalCanciones;
    mutable unsigned long iteraciones;

    void limpiarArtistas();
    void limpiarAlbumes();
    void limpiarCanciones();

    bool agregarArtista(Artista* artista);
    bool agregarAlbum(Album* album);
    bool agregarCancion(Cancion* cancion);

public:
    GestorCatalogo();
    ~GestorCatalogo();

    Artista* buscarArtista(int id) const;
    Album* buscarAlbum(int id) const;
    Cancion* buscarCancion(int id) const;

    void cargarArtistas();
    void cargarAlbumes();
    void cargarCanciones();
    void guardarCanciones() const;

    int getTotalArtistas() const { return totalArtistas; }
    int getTotalAlbumes() const { return totalAlbumes; }
    int getTotalCanciones() const { return totalCanciones; }
    Cancion** getCancionesArray() const;

    unsigned long getIteraciones() const { return iteraciones; }
    void incrementarIteraciones(int cantidad = 1) const { iteraciones += cantidad; }
    void resetIteraciones() const { iteraciones = 0; }
    unsigned long calcularMemoriaArtistas() const;
    unsigned long calcularMemoriaAlbumes() const;
    unsigned long calcularMemoriaCanciones() const;

    void limpiarCatalogo();

};

#endif
