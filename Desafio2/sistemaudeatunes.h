#ifndef SISTEMAUDEATUNES_H
#define SISTEMAUDEATUNES_H

#include <string>
#include "GestorUsuarios.h"

class Artista;
class Album;
class Cancion;
class MensajePublicitario;
class Reproductor;
class ListaFavoritos;
class GestorCatalogo;
class GestorPersistencia;

class SistemaUdeATunes {
private:
    GestorUsuarios* gestorUsuarios;

    Artista** artistas;
    Album** albumes;
    Cancion** canciones;
    MensajePublicitario** mensajes;

    Usuario* usuarioActual;
    Reproductor* reproductor;

    int totalArtistas;
    int totalAlbumes;
    int totalCanciones;
    int totalMensajes;
    int capacidad;

    void redimensionarArtistas();
    void redimensionarAlbumes();
    void redimensionarCanciones();
    void redimensionarMensajes();

    void cargarArtistas();
    void cargarAlbumes();
    void cargarCanciones();
    void cargarMensajes();
    void guardarArtistas();
    void guardarAlbumes();
    void guardarCanciones();
    void guardarMensajes();

public:
    SistemaUdeATunes();
    ~SistemaUdeATunes();

    bool login();
    void reproducirAleatorio();
    void mostrarMetricas() const;
    void mostrarMetricasEficiencia() const;

    Usuario* buscarUsuario(const std::string& nickname) const;
    Cancion* buscarCancion(int id) const;
    Artista* buscarArtista(int id) const;
    Album* buscarAlbum(int id) const;

    bool agregarUsuario(Usuario* usuario);
    bool agregarArtista(Artista* artista);
    bool agregarAlbum(Album* album);
    bool agregarCancion(Cancion* cancion);
    bool agregarMensaje(MensajePublicitario* mensaje);

    Usuario* getUsuarioActual() const { return usuarioActual; }
    int getTotalUsuarios() const { return gestorUsuarios->getTotalUsuarios(); }
    int getTotalCanciones() const { return totalCanciones; }
    int getTotalArtistas() const { return totalArtistas; }
    int getTotalAlbumes() const { return totalAlbumes; }
    int getTotalMensajes() const { return totalMensajes; }
    Cancion** getCanciones() const { return canciones; }
    MensajePublicitario** getMensajes() const { return mensajes; }

    void cargarDatos();
    void guardarDatos();

    void setReproductor(Reproductor* reprod) { reproductor = reprod; }
    Reproductor* getReproductor() const { return reproductor; }

    void setUsuarioActual(Usuario* usuario) { usuarioActual = usuario; }
    void mostrarCancionesDisponibles() const;
    bool agregarCancionAFavoritos(int idCancion);

    bool seguirListaUsuario(const std::string& nicknameSeguido);
    void dejarDeSeguirLista();
    void combinarListaSeguida();
    void mostrarInfoListaSeguida() const;

    void incrementarIteraciones(int cantidad = 1) const;
    void calcularMemoria() const;

private:
    mutable unsigned long totalIteraciones;
    mutable unsigned long memoriaConsumida;
};

#endif
