#include "Usuario.h"
#include "ListaFavoritos.h"
#include <iostream>

Usuario::Usuario()
    : nickname(""), membresia("estandar"), listaFavoritos(nullptr) {
}

Usuario::Usuario(const std::string& nickname, const std::string& membresia)
    : nickname(nickname), membresia(membresia), listaFavoritos(nullptr) {
    if (membresia == "premium") {
        listaFavoritos = new ListaFavoritos(this);
    }
}

Usuario::~Usuario() {
    if (listaFavoritos != nullptr) {
        delete listaFavoritos;
    }
}

bool Usuario::esPremium() const {
    return membresia == "premium";
}
