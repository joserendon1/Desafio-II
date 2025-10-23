#ifndef SISTEMAUDEATUNES_H
#define SISTEMAUDEATUNES_H

#include <string>
#include "GestorUsuarios.h"
#include "GestorCatalogo.h"
#include "MensajePublicitario.h"
#include "reproductor.h"

class SistemaUdeATunes {
private:
    GestorUsuarios* gestorUsuarios;
    GestorCatalogo* gestorCatalogo;

    MensajePublicitario** mensajes;
    int totalMensajes;
    int capacidadMensajes;

    Usuario* usuarioActual;
    Reproductor* reproductor;

    mutable unsigned long totalIteraciones;
    mutable unsigned long memoriaConsumida;

    void redimensionarMensajes();
    void cargarMensajes();
    void guardarCambios() const;

public:
    SistemaUdeATunes();
    ~SistemaUdeATunes();

    void cargarDatos();

    bool login();
    void reproducirAleatorio();
    void mostrarMetricas() const;
    void mostrarMetricasEficiencia() const;

    Usuario* buscarUsuario(const std::string& nickname) const;
    Cancion* buscarCancion(int id) const;
    Artista* buscarArtista(int id) const;
    Album* buscarAlbum(int id) const;

    Usuario* getUsuarioActual() const { return usuarioActual; }
    int getTotalUsuarios() const { return gestorUsuarios->getTotalUsuarios(); }
    int getTotalCanciones() const { return gestorCatalogo->getTotalCanciones(); }
    int getTotalArtistas() const { return gestorCatalogo->getTotalArtistas(); }
    int getTotalAlbumes() const { return gestorCatalogo->getTotalAlbumes(); }
    int getTotalMensajes() const { return totalMensajes; }
    Cancion** getCanciones() const { return gestorCatalogo->getCanciones(); }
    MensajePublicitario** getMensajes() const { return mensajes; }

    void setUsuarioActual(Usuario* usuario) { usuarioActual = usuario; }
    void mostrarCancionesDisponibles() const;
    bool agregarCancionAFavoritos(int idCancion);

    bool seguirListaUsuario(const std::string& nicknameSeguido);
    void dejarDeSeguirLista();

    void incrementarIteraciones(int cantidad = 1) const;
    void calcularMemoria() const;
};

#endif
