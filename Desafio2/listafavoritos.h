#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

class Usuario;
class Cancion;

class ListaFavoritos {
private:
    struct ContenedorCancionFavorita {
        Cancion* contenido;
        ContenedorCancionFavorita* siguiente;
        ContenedorCancionFavorita(Cancion* c) : contenido(c), siguiente(nullptr) {}
    };

    ContenedorCancionFavorita* inicio;
    int totalCanciones;
    Usuario* usuario;
    ListaFavoritos* listaSeguida;

public:
    ListaFavoritos(Usuario* usuario);
    ~ListaFavoritos();

    bool agregarCancion(Cancion* cancion);
    bool eliminarCancion(int idCancion);
    bool contieneCancion(int idCancion) const;

    void reproducir(bool ordenAleatorio) const;
    void seguirLista(ListaFavoritos* otraLista);
    void dejarDeSeguirLista();
    void mostrarLista() const;

    int getTotalCanciones() const { return totalCanciones; }
    int getTotalCancionesSeguidas() const;
    int getTotalCancionesVisibles() const;
    Cancion* obtenerCancion(int index) const;

    ListaFavoritos* getListaSeguida() const { return listaSeguida; }
    bool estaSiguiendoLista() const { return listaSeguida != nullptr; }

    Usuario* getUsuario() const { return usuario; }
    Cancion** getCancionesArray() const;
};

#endif
