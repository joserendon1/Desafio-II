#include "Creditos.h"
#include <string>

Creditos::Creditos() {
    capacidad = 10;
    productores = new std::string[capacidad];
    musicos = new std::string[capacidad];
    compositores = new std::string[capacidad];
    totalProductores = 0;
    totalMusicos = 0;
    totalCompositores = 0;
}

Creditos::~Creditos() {
    delete[] productores;
    delete[] musicos;
    delete[] compositores;
}

Creditos& Creditos::operator=(const Creditos& otros) {
    if (this != &otros) {

        delete[] productores;
        delete[] musicos;
        delete[] compositores;

        capacidad = otros.capacidad;
        totalProductores = otros.totalProductores;
        totalMusicos = otros.totalMusicos;
        totalCompositores = otros.totalCompositores;

        productores = new std::string[capacidad];
        musicos = new std::string[capacidad];
        compositores = new std::string[capacidad];

        for (int i = 0; i < totalProductores; i++) {
            productores[i] = otros.productores[i];
        }
        for (int i = 0; i < totalMusicos; i++) {
            musicos[i] = otros.musicos[i];
        }
        for (int i = 0; i < totalCompositores; i++) {
            compositores[i] = otros.compositores[i];
        }
    }
    return *this;
}

void Creditos::redimensionarProductores() {
    int nuevaCapacidad = capacidad * 2;
    std::string* nuevoArray = new std::string[nuevaCapacidad];

    for (int i = 0; i < totalProductores; i++) {
        nuevoArray[i] = productores[i];
    }

    delete[] productores;
    productores = nuevoArray;
    capacidad = nuevaCapacidad;
}

void Creditos::redimensionarMusicos() {
    int nuevaCapacidad = capacidad * 2;
    std::string* nuevoArray = new std::string[nuevaCapacidad];

    for (int i = 0; i < totalMusicos; i++) {
        nuevoArray[i] = musicos[i];
    }

    delete[] musicos;
    musicos = nuevoArray;
    capacidad = nuevaCapacidad;
}

void Creditos::redimensionarCompositores() {
    int nuevaCapacidad = capacidad * 2;
    std::string* nuevoArray = new std::string[nuevaCapacidad];

    for (int i = 0; i < totalCompositores; i++) {
        nuevoArray[i] = compositores[i];
    }

    delete[] compositores;
    compositores = nuevoArray;
    capacidad = nuevaCapacidad;
}

void Creditos::agregarProductor(const std::string& productor) {
    if (totalProductores >= capacidad) {
        redimensionarProductores();
    }
    productores[totalProductores++] = productor;
}

void Creditos::agregarMusico(const std::string& musico) {
    if (totalMusicos >= capacidad) {
        redimensionarMusicos();
    }
    musicos[totalMusicos++] = musico;
}

void Creditos::agregarCompositor(const std::string& compositor) {
    if (totalCompositores >= capacidad) {
        redimensionarCompositores();
    }
    compositores[totalCompositores++] = compositor;
}
