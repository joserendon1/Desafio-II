#include "Album.h"
#include "Artista.h"
#include <string>

Album::Album()
    : id(0), nombre(""), artista(nullptr), portada("") {
}

Album::Album(int id, const std::string& nombre, Artista* artista)
    : id(id), nombre(nombre), artista(artista), portada("") {
}

Album::~Album() {
}
