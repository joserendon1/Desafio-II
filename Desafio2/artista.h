#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>

class Album;

class Artista {
private:
    int id;
    std::string nombre;
    int edad;
    std::string pais;
    int seguidores;
    int posicion;

public:
    Artista();
    Artista(int id, const std::string& nombre);

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    int getEdad() const { return edad; }
    std::string getPais() const { return pais; }
    int getSeguidores() const { return seguidores; }
    int getPosicion() const { return posicion; }

    void setId(int id) { this->id = id; }
    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setEdad(int edad) { this->edad = edad; }
    void setPais(const std::string& pais) { this->pais = pais; }
    void setSeguidores(int seguidores) { this->seguidores = seguidores; }
    void setPosicion(int posicion) { this->posicion = posicion; }
};

#endif
