#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>

class Album;

class Artista {
private:
    int id;
    std::string nombre;

public:
    Artista();
    Artista(int id, const std::string& nombre);

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }

    void setId(int id) { this->id = id; }
    void setNombre(const std::string& nombre) { this->nombre = nombre; }
};

#endif
