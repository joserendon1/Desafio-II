#include "Album.h"
#include "Artista.h"
#include <string>

Album::Album()
    : id(0), nombre(""), artista(nullptr), fecha(""), duracion(0.0f),
    sello(""), portada(""), puntuacion(0.0f), totalGeneros(0), capacidadGeneros(5) {
    generos = new std::string[capacidadGeneros];
}

Album::Album(int id, const std::string& nombre, Artista* artista)
    : id(id), nombre(nombre), artista(artista), fecha(""), duracion(0.0f),
    sello(""), portada(""), puntuacion(0.0f), totalGeneros(0), capacidadGeneros(5) {
    generos = new std::string[capacidadGeneros];
}

Album::~Album() {
    delete[] generos;
}

void Album::redimensionarGeneros() {
    int nuevaCapacidad = capacidadGeneros * 2;
    std::string* nuevosGeneros = new std::string[nuevaCapacidad];

    for (int i = 0; i < totalGeneros; i++) {
        nuevosGeneros[i] = generos[i];
    }

    delete[] generos;
    generos = nuevosGeneros;
    capacidadGeneros = nuevaCapacidad;
}

void Album::agregarGenero(const std::string& genero) {
    if (totalGeneros >= capacidadGeneros) {
        redimensionarGeneros();
    }

    if (totalGeneros < 4) {
        generos[totalGeneros++] = genero;
    }
}
