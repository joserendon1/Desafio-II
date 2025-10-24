#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class ListaFavoritos;

class Usuario {
private:
    std::string nickname;
    std::string membresia;
    ListaFavoritos* listaFavoritos;

public:
    Usuario();
    Usuario(const std::string& nickname, const std::string& membresia);
    ~Usuario();

    bool esPremium() const;

    std::string getNickname() const { return nickname; }
    std::string getMembresia() const { return membresia; }
    ListaFavoritos* getListaFavoritos() const { return listaFavoritos; }
};

#endif
