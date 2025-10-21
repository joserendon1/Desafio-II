#ifndef SISTEMAUDEATUNES_H
#define SISTEMAUDEATUNES_H

#include <string>

class Usuario;
class Artista;
class Album;
class Cancion;
class MensajePublicitario;
class Reproductor;

class SistemaUdeATunes {
private:
    Usuario** usuarios;
    Artista** artistas;
    Album** albumes;
    Cancion** canciones;
    MensajePublicitario** mensajes;
    Usuario* usuarioActual;
    Reproductor* reproductor;

    int totalUsuarios;
    int totalArtistas;
    int totalAlbumes;
    int totalCanciones;
    int totalMensajes;
    int capacidad;

    void redimensionarUsuarios();
    void redimensionarArtistas();
    void redimensionarAlbumes();
    void redimensionarCanciones();
    void redimensionarMensajes();

    void cargarUsuarios();
    void cargarArtistas();
    void cargarAlbumes();
    void cargarCanciones();
    void cargarMensajes();

    void guardarUsuarios();
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
    int getTotalUsuarios() const { return totalUsuarios; }
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
};

#endif
