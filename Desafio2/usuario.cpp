#include "Cancion.h"
#include "Album.h"
#include <iostream>

Cancion::Cancion()
    : id(0), nombre(""), duracion(0.0f), ruta128(""), ruta320(""),
    reproducciones(0), album(nullptr) {
    creditos = new Creditos();
}

Cancion::Cancion(int id, const std::string& nombre, Album* album)
    : id(id), nombre(nombre), duracion(0.0f), ruta128(""), ruta320(""),
    reproducciones(0), album(album) {
    creditos = new Creditos();
}

Cancion::~Cancion() {
    delete creditos;
}

bool Cancion::operator==(const Cancion& otra) const {
    return id == otra.id;
}

void Cancion::reproducir() {
    reproducciones++;
    std::cout << "Reproduciendo: " << nombre << std::endl;
}

std::string Cancion::obtenerRuta(bool altaCalidad) const {
    return altaCalidad ? ruta320 : ruta128;
}
