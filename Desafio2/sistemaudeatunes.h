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

    struct ContenedorMensaje {
        MensajePublicitario* contenido;
        ContenedorMensaje* siguiente;
        ContenedorMensaje(MensajePublicitario* m) : contenido(m), siguiente(nullptr) {}
    };

    ContenedorMensaje* inicioMensajes;
    int totalMensajes;

    Usuario* usuarioActual;
    Reproductor* reproductor;

    mutable unsigned long totalIteraciones;
    mutable unsigned long memoriaConsumida;
    unsigned long calcularMemoriaMensajes() const;
    unsigned long calcularMemoriaTodasListasFavoritos() const;

    void cargarMensajes();
    void guardarCambios() const;
    void limpiarMensajes();

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

    Cancion** getCanciones() const { return gestorCatalogo->getCancionesArray(); }
    MensajePublicitario** getMensajesArray() const;
    MensajePublicitario** getMensajes() const { return getMensajesArray(); }

    void setUsuarioActual(Usuario* usuario) { usuarioActual = usuario; }
    void mostrarCancionesDisponibles() const;
    bool agregarCancionAFavoritos(int idCancion);

    bool seguirListaUsuario(const std::string& nicknameSeguido);
    void dejarDeSeguirLista();

    void incrementarIteraciones(int cantidad = 1) const;
    void resetIteraciones() const {
        totalIteraciones = 0;
        gestorUsuarios->resetIteraciones();
        gestorCatalogo->resetIteraciones();
    }

    unsigned long getTotalIteraciones() const {
        return totalIteraciones + gestorUsuarios->getIteraciones() + gestorCatalogo->getIteraciones();
    }
    void calcularMemoria() const;
};

#endif
