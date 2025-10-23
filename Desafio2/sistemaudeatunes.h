#ifndef SISTEMAUDEATUNES_H
#define SISTEMAUDEATUNES_H

#include <string>
#include "GestorUsuarios.h"
#include "GestorCatalogo.h"
#include "GestorAlmacenamiento.h"
#include "reproductor.h"

class SistemaUdeATunes {
private:
    GestorUsuarios* gestorUsuarios;
    GestorCatalogo* gestorCatalogo;
    GestorAlmacenamiento* gestorAlmacenamiento;

    Usuario* usuarioActual;
    Reproductor* reproductor;

    mutable unsigned long totalIteraciones;
    mutable unsigned long memoriaConsumida;

public:
    SistemaUdeATunes();
    ~SistemaUdeATunes();

    void cargarDatos();
    void guardarDatos();

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

    Usuario* getUsuarioActual() const { return usuarioActual; }
    int getTotalUsuarios() const { return gestorUsuarios->getTotalUsuarios(); }
    int getTotalCanciones() const { return gestorCatalogo->getTotalCanciones(); }
    int getTotalArtistas() const { return gestorCatalogo->getTotalArtistas(); }
    int getTotalAlbumes() const { return gestorCatalogo->getTotalAlbumes(); }
    int getTotalMensajes() const { return gestorAlmacenamiento->getTotalMensajes(); }
    Cancion** getCanciones() const { return gestorCatalogo->getCanciones(); }
    MensajePublicitario** getMensajes() const { return gestorAlmacenamiento->getMensajes(); }

    void setUsuarioActual(Usuario* usuario) { usuarioActual = usuario; }
    void mostrarCancionesDisponibles() const;
    bool agregarCancionAFavoritos(int idCancion);

    bool seguirListaUsuario(const std::string& nicknameSeguido);
    void dejarDeSeguirLista();

    void incrementarIteraciones(int cantidad = 1) const;
    void calcularMemoria() const;
};

#endif
