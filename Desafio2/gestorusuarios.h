#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

#include "Usuario.h"
#include <string>

class GestorUsuarios {
private:
    struct ContenedorUsuario {
        Usuario* contenido;
        ContenedorUsuario* siguiente;
        ContenedorUsuario(Usuario* u) : contenido(u), siguiente(nullptr) {}
    };

    ContenedorUsuario* inicio;
    ContenedorUsuario* final;
    int totalUsuarios;
    mutable unsigned long iteraciones;

public:
    GestorUsuarios();
    ~GestorUsuarios();

    Usuario* buscarUsuario(const std::string& nickname) const;
    bool agregarUsuario(Usuario* usuario);
    void cargarUsuarios();

    int getTotalUsuarios() const { return totalUsuarios; }
    Usuario** getUsuariosArray() const;
    unsigned long getIteraciones() const { return iteraciones; }

    void incrementarIteraciones(int cantidad = 1) const { iteraciones += cantidad; }
    void resetIteraciones() const { iteraciones = 0; }
    void limpiarUsuarios();
    unsigned long calcularMemoriaUsuarios() const;
};

#endif
