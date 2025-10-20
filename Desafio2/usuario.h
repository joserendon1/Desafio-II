#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class ListaFavoritos;

class Usuario {
private:
    std::string nickname;
    std::string membresia;
    std::string ciudad;
    std::string pais;
    std::string fechaInscripcion;
    ListaFavoritos* listaFavoritos;

public:
    Usuario();
    Usuario(const std::string& nickname, const std::string& membresia);
    ~Usuario();

    bool operator==(const Usuario& otro) const;

    bool esPremium() const;
    void cambiarMembresia(const std::string& nuevaMembresia);

    std::string getNickname() const { return nickname; }
    std::string getMembresia() const { return membresia; }
    std::string getCiudad() const { return ciudad; }
    std::string getPais() const { return pais; }
    ListaFavoritos* getListaFavoritos() const { return listaFavoritos; }

    void setNickname(const std::string& nickname) { this->nickname = nickname; }
    void setCiudad(const std::string& ciudad) { this->ciudad = ciudad; }
    void setPais(const std::string& pais) { this->pais = pais; }
    void setFechaInscripcion(const std::string& fecha) { fechaInscripcion = fecha; }
};

#endif
