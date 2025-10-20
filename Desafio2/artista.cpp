#include "Artista.h"
#include <string>

Artista::Artista()
    : id(0), nombre(""), edad(0), pais(""), seguidores(0), posicion(0) {
}

Artista::Artista(int id, const std::string& nombre)
    : id(id), nombre(nombre), edad(0), pais(""), seguidores(0), posicion(0) {
}
