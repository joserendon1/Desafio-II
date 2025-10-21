#ifndef ALBUM_H
#define ALBUM_H

#include <string>

class Artista;

class Album {
private:
    int id;
    std::string nombre;
    Artista* artista;
    std::string fecha;
    float duracion;
    std::string sello;
    std::string portada;
    float puntuacion;
    std::string* generos;
    int totalGeneros;
    int capacidadGeneros;

    void redimensionarGeneros();

public:
    Album();
    Album(int id, const std::string& nombre, Artista* artista);
    ~Album();

    void agregarGenero(const std::string& genero);

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    Artista* getArtista() const { return artista; }
    std::string getFecha() const { return fecha; }
    float getDuracion() const { return duracion; }
    std::string getSello() const { return sello; }
    std::string getPortada() const { return portada; }
    float getPuntuacion() const { return puntuacion; }
    std::string* getGeneros() const { return generos; }
    int getTotalGeneros() const { return totalGeneros; }

    void setId(int id) { this->id = id; }
    void setNombre(const std::string& nombre) { this->nombre = nombre; }
    void setArtista(Artista* artista) { this->artista = artista; }
    void setFecha(const std::string& fecha) { this->fecha = fecha; }
    void setDuracion(float duracion) { this->duracion = duracion; }
    void setSello(const std::string& sello) { this->sello = sello; }
    void setPortada(const std::string& portada) { this->portada = portada; }
    void setPuntuacion(float puntuacion) { this->puntuacion = puntuacion; }
};

#endif
