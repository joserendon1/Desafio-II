#ifndef CREDITOS_H
#define CREDITOS_H

#include <string>

class Creditos {
private:
    std::string* productores;
    std::string* musicos;
    std::string* compositores;
    int totalProductores;
    int totalMusicos;
    int totalCompositores;
    int capacidad;

    void redimensionarProductores();
    void redimensionarMusicos();
    void redimensionarCompositores();

public:
    Creditos();
    ~Creditos();

    Creditos& operator=(const Creditos& otros);

    void agregarProductor(const std::string& productor);
    void agregarMusico(const std::string& musico);
    void agregarCompositor(const std::string& compositor);

    int getTotalProductores() const { return totalProductores; }
    int getTotalMusicos() const { return totalMusicos; }
    int getTotalCompositores() const { return totalCompositores; }
    std::string* getProductores() const { return productores; }
    std::string* getMusicos() const { return musicos; }
    std::string* getCompositores() const { return compositores; }
};

#endif
