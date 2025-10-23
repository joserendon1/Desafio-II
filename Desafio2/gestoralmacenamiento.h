#ifndef GESTORALMACENAMIENTO_H
#define GESTORALMACENAMIENTO_H

#include "GestorUsuarios.h"
#include "GestorCatalogo.h"
#include "MensajePublicitario.h"
#include <string>

class GestorAlmacenamiento {
private:
    GestorUsuarios* gestorUsuarios;
    GestorCatalogo* gestorCatalogo;

    MensajePublicitario** mensajes;
    int totalMensajes;
    int capacidad;

    void redimensionarMensajes();
    void cargarMensajes();

public:
    GestorAlmacenamiento(GestorUsuarios* gestorUsuarios, GestorCatalogo* gestorCatalogo);
    ~GestorAlmacenamiento();

    void cargarTodo();
    void guardarCambios() const;  // CAMBIAR NOMBRE: solo guarda lo que cambia

    MensajePublicitario** getMensajes() const { return mensajes; }
    int getTotalMensajes() const { return totalMensajes; }

    void limpiarMensajes();
};

#endif
