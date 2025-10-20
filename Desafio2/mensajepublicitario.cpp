#include "MensajePublicitario.h"

MensajePublicitario::MensajePublicitario()
    : texto(""), categoria('C'), prioridad(1) {
}

MensajePublicitario::MensajePublicitario(const std::string& texto, char categoria)
    : texto(texto), categoria(categoria) {
    calcularPrioridad();
}

bool MensajePublicitario::operator==(const MensajePublicitario& otro) const {
    return (texto == otro.texto && categoria == otro.categoria);
}

void MensajePublicitario::setCategoria(char categoria) {
    this->categoria = categoria;
    calcularPrioridad();
}

void MensajePublicitario::calcularPrioridad() {
    switch (categoria) {
    case 'A': prioridad = 3; break;
    case 'B': prioridad = 2; break;
    case 'C': prioridad = 1; break;
    default: prioridad = 1; break;
    }
}
