#include "Cancion.h"
#include "Album.h"
#include <iostream>

Cancion::Cancion()
    : id(0), nombre(""), duracion(0.0f), ruta128(""), ruta320(""),
    reproducciones(0), album(nullptr) {
}

Cancion::Cancion(int id, const std::string& nombre, Album* album)
    : id(id), nombre(nombre), duracion(0.0f), ruta128(""), ruta320(""),
    reproducciones(0), album(album) {
}

Cancion::~Cancion() {
}

bool Cancion::operator==(const Cancion& otra) const {
    return id == otra.id;
}

std::string Cancion::obtenerRuta(bool altaCalidad) const {
    return altaCalidad ? ruta320 : ruta128;
}

