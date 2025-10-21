#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

class Usuario;
class Cancion;

class ListaFavoritos {
private:
    Cancion** canciones;
    int totalCanciones;
    int capacidad;
    Usuario* usuario;
    ListaFavoritos* listaSeguida;

    void redimensionar();

public:
    ListaFavoritos(Usuario* usuario);
    ~ListaFavoritos();

    bool agregarCancion(Cancion* cancion);
    bool eliminarCancion(int idCancion);
    bool contieneCancion(int idCancion) const;

    void reproducir(bool ordenAleatorio) const;
    void seguirLista(ListaFavoritos* otraLista);
    void dejarDeSeguirLista();
    void combinarConListaSeguida();
    void mostrarLista() const;

    ListaFavoritos* getListaSeguida() const { return listaSeguida; }
    bool estaSiguiendoLista() const { return listaSeguida != nullptr; }

    int getTotalCanciones() const { return totalCanciones; }
    int getCapacidad() const { return capacidad; }
    Usuario* getUsuario() const { return usuario; }
    Cancion** getCanciones() const { return canciones; }
};

#endif
