#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>

struct Artista {
    int id;
    std::string nombre;

    Artista(int id = 0, const std::string& nombre = "")
        : id(id), nombre(nombre) {}
};

#endif
