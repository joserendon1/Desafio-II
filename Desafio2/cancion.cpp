#include "Usuario.h"
#include "ListaFavoritos.h"
#include <iostream>

Usuario::Usuario()
    : nickname(""), membresia("estándar"), ciudad(""), pais(""),
    fechaInscripcion(""), listaFavoritos(nullptr) {
}

Usuario::Usuario(const std::string& nickname, const std::string& membresia)
    : nickname(nickname), membresia(membresia), ciudad(""), pais(""),
    fechaInscripcion("") {
    if (membresia == "premium") {
        listaFavoritos = new ListaFavoritos(this);
    } else {
        listaFavoritos = nullptr;
    }
}

Usuario::~Usuario() {
    if (listaFavoritos != nullptr) {
        delete listaFavoritos;
    }
}

bool Usuario::operator==(const Usuario& otro) const {
    return nickname == otro.nickname;
}

bool Usuario::esPremium() const {
    return membresia == "premium";
}

void Usuario::cambiarMembresia(const std::string& nuevaMembresia) {
    membresia = nuevaMembresia;

    if (membresia == "premium" && listaFavoritos == nullptr) {
        listaFavoritos = new ListaFavoritos(this);
    } else if (membresia != "premium" && listaFavoritos != nullptr) {
        delete listaFavoritos;
        listaFavoritos = nullptr;
    }
}
