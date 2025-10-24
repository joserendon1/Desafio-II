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

    Usuario* buscarUsuario(const std::string& nickname) const;
    Cancion* buscarCancion(int id) const;

    Usuario* getUsuarioActual() const { return usuarioActual; }
    int getTotalUsuarios() const { return gestorUsuarios->getTotalUsuarios(); }
    int getTotalCanciones() const { return gestorCatalogo->getTotalCanciones(); }
    int getTotalArtistas() const { return gestorCatalogo->getTotalArtistas(); }
    int getTotalAlbumes() const { return gestorCatalogo->getTotalAlbumes(); }
    int getTotalMensajes() const { return totalMensajes; }

    Cancion** getCanciones() const { return gestorCatalogo->getCancionesArray(); }
    MensajePublicitario** getMensajesArray() const;

    void setUsuarioActual(Usuario* usuario) { usuarioActual = usuario; }
    void mostrarCancionesDisponibles() const;
    bool agregarCancionAFavoritos(int idCancion);
    bool seguirListaUsuario(const std::string& nicknameSeguido);
    void dejarDeSeguirLista();

    void mostrarMetricasEficiencia() const;
    void incrementarIteraciones(int cantidad = 1) const;
    void resetIteraciones() const;
    unsigned long getTotalIteraciones() const;
    void calcularMemoria() const;

};

#endif
