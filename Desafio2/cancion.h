#ifndef CANCION_H
#define CANCION_H

#include <string>

struct Album;

class Cancion {
private:
    int id;
    std::string nombre;
    float duracion;
    std::string ruta128;
    std::string ruta320;
    int reproducciones;
    Album* album;

public:
    Cancion();
    Cancion(int id, const std::string& nombre, Album* album);
    ~Cancion();

    bool operator==(const Cancion& otra) const;

    std::string obtenerRuta(bool altaCalidad) const;
    void incrementarReproducciones() { reproducciones++; }

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    float getDuracion() const { return duracion; }
    int getReproducciones() const { return reproducciones; }
    Album* getAlbum() const { return album; }

    void setDuracion(float duracion) { this->duracion = duracion; }
    void setRuta128(const std::string& ruta) { ruta128 = ruta; }
    void setRuta320(const std::string& ruta) { ruta320 = ruta; }

};

#endif
