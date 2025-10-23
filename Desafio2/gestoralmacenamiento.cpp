#include "GestorAlmacenamiento.h"
#include <iostream>
#include <fstream>
#include <sstream>

GestorAlmacenamiento::GestorAlmacenamiento(GestorUsuarios* gestorUsuarios, GestorCatalogo* gestorCatalogo)
    : gestorUsuarios(gestorUsuarios), gestorCatalogo(gestorCatalogo),
    mensajes(nullptr), totalMensajes(0), capacidad(10) {

    mensajes = new MensajePublicitario*[capacidad];
}

GestorAlmacenamiento::~GestorAlmacenamiento() {
    limpiarMensajes();
    delete[] mensajes;
}

void GestorAlmacenamiento::limpiarMensajes() {
    for (int i = 0; i < totalMensajes; i++) {
        delete mensajes[i];
    }
    totalMensajes = 0;
}

void GestorAlmacenamiento::redimensionarMensajes() {
    int nuevaCapacidad = capacidad * 2;
    MensajePublicitario** nuevoArray = new MensajePublicitario*[nuevaCapacidad];

    for (int i = 0; i < totalMensajes; i++) {
        nuevoArray[i] = mensajes[i];
    }

    delete[] mensajes;
    mensajes = nuevoArray;
    capacidad = nuevaCapacidad;
}

void GestorAlmacenamiento::cargarTodo() {
    std::cout << "=== CARGANDO DATOS DEL SISTEMA ===" << std::endl;

    gestorUsuarios->cargarUsuarios();
    gestorCatalogo->cargarArtistas();
    gestorCatalogo->cargarAlbumes();
    gestorCatalogo->cargarCanciones();
    cargarMensajes();

    std::cout << "=== CARGA DE DATOS COMPLETADA ===" << std::endl;
}

void GestorAlmacenamiento::guardarCambios() const {
    std::cout << "=== GUARDANDO CAMBIOS ===" << std::endl;

    gestorCatalogo->guardarCanciones();

    std::cout << "=== GUARDADO DE CAMBIOS COMPLETADO ===" << std::endl;
}

void GestorAlmacenamiento::cargarMensajes() {
    std::ifstream archivo("datos/mensajes.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de mensajes." << std::endl;
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t pos = linea.find_last_of('|');
        if (pos == std::string::npos) continue;

        std::string texto = linea.substr(0, pos);
        char categoria = linea[pos + 1];

        MensajePublicitario* nuevoMensaje = new MensajePublicitario(texto, categoria);
        if (totalMensajes >= capacidad) {
            redimensionarMensajes();
        }
        mensajes[totalMensajes++] = nuevoMensaje;
    }

    archivo.close();
    std::cout << "Mensajes cargados: " << totalMensajes << std::endl;
}

