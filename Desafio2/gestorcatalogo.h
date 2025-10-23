#ifndef GESTORCATALOGO_H
#define GESTORCATALOGO_H

#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include <string>

class GestorCatalogo {
private:
    Artista** artistas;
    Album** albumes;
    Cancion** canciones;
    int totalArtistas;
    int totalAlbumes;
    int totalCanciones;
    int capacidad;
    mutable unsigned long iteraciones;

    void redimensionarArtistas();
    void redimensionarAlbumes();
    void redimensionarCanciones();

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
    Artista** getArtistas() const { return artistas; }
    Album** getAlbumes() const { return albumes; }
    Cancion** getCanciones() const { return canciones; }
    unsigned long getIteraciones() const { return iteraciones; }
    void incrementarIteraciones(int cantidad = 1) const { iteraciones += cantidad; }

    void limpiarCatalogo();
};

#endif
