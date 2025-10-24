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
    mutable unsigned long iteraciones;

    int getTotalCancionesSeguidas() const;

public:
    ListaFavoritos(Usuario* usuario);
    ~ListaFavoritos();

    bool agregarCancion(Cancion* cancion);
    bool eliminarCancion(int idCancion);
    bool contieneCancion(int idCancion) const;

    Cancion** getCancionesConSeguidas(bool ordenAleatorio = false) const;
    int getTotalCancionesVisibles() const;

    void seguirLista(ListaFavoritos* otraLista);
    void dejarDeSeguirLista();
    void mostrarLista() const;

    int getTotalCanciones() const { return totalCanciones; }
    Usuario* getUsuario() const { return usuario; }
    Cancion** getCancionesArray() const;
    bool estaSiguiendoLista() const { return listaSeguida != nullptr; }

    unsigned long getIteraciones() const { return iteraciones; }
    void resetIteraciones() const { iteraciones = 0; }
    void incrementarIteraciones(int cantidad = 1) const { iteraciones += cantidad; }
    unsigned long calcularMemoria() const;
};

#endif
