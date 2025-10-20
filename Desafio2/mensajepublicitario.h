#ifndef MENSAJEPUBLICITARIO_H
#define MENSAJEPUBLICITARIO_H

#include <string>

class MensajePublicitario {
private:
    std::string texto;
    char categoria;
    int prioridad;

public:
    MensajePublicitario();
    MensajePublicitario(const std::string& texto, char categoria);

    bool operator==(const MensajePublicitario& otro) const;

    std::string getTexto() const { return texto; }
    char getCategoria() const { return categoria; }
    int getPrioridad() const { return prioridad; }

    void setTexto(const std::string& texto) { this->texto = texto; }
    void setCategoria(char categoria);

private:
    void calcularPrioridad();
};

#endif
