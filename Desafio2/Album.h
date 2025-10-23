#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "Artista.h"

struct Album {
    int id;
    std::string nombre;
    Artista* artista;
    std::string portada;

    Album(int id = 0, const std::string& nombre = "", Artista* artista = nullptr)
        : id(id), nombre(nombre), artista(artista), portada("") {}
};

#endif
