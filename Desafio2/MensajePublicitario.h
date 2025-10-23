#ifndef MENSAJEPUBLICITARIO_H
#define MENSAJEPUBLICITARIO_H

#include <string>

struct MensajePublicitario {
    std::string texto;
    char categoria;
    int prioridad;

    MensajePublicitario(const std::string& texto = "", char categoria = 'C')
        : texto(texto), categoria(categoria) {
        calcularPrioridad();
    }

    bool operator==(const MensajePublicitario& otro) const {
        return (texto == otro.texto && categoria == otro.categoria);
    }

    void setCategoria(char cat) {
        categoria = cat;
        calcularPrioridad();
    }

private:
    void calcularPrioridad() {
        switch(categoria) {
        case 'A': prioridad = 3; break;
        case 'B': prioridad = 2; break;
        case 'C': prioridad = 1; break;
        default: prioridad = 1; break;
        }
    }
};

#endif
