#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

#include "Usuario.h"
#include <string>

class GestorUsuarios {
private:
    Usuario** usuarios;
    int totalUsuarios;
    int capacidad;
    mutable unsigned long iteraciones;

    void redimensionar();
    bool agregarUsuario(Usuario* usuario);

public:
    GestorUsuarios();
    ~GestorUsuarios();

    Usuario* buscarUsuario(const std::string& nickname) const;
    bool autenticarUsuario(const std::string& nickname, Usuario*& usuarioActual) const;

    void cargarUsuarios();

    int getTotalUsuarios() const { return totalUsuarios; }
    Usuario** getUsuarios() const { return usuarios; }
    unsigned long getIteraciones() const { return iteraciones; }
    void incrementarIteraciones(int cantidad = 1) const { iteraciones += cantidad; }

    bool existeUsuario(const std::string& nickname) const;
    void limpiarUsuarios();
};

#endif
