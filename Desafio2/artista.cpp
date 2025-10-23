#include "Artista.h"
#include <string>

Artista::Artista()
    : id(0), nombre("") {
}

Artista::Artista(int id, const std::string& nombre)
    : id(id), nombre(nombre) {
}
