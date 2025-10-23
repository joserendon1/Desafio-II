#ifndef ALBUM_H
#define ALBUM_H

#include <string>

class Artista;

class Album {
private:
    int id;
    std::string nombre;
    Artista* artista;
    std::string portada;

public:
    Album();
    Album(int id, const std::string& nombre, Artista* artista);
    ~Album();

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    Artista* getArtista() const { return artista; }
    std::string getPortada() const { return portada; }

    void setId(int id) { this->id = id; }
    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setArtista(Artista* artista) { this->artista = artista; }
    void setPortada(const std::string& portada) { this->portada = portada; }
};

#endif
