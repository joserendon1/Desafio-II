TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cancion.cpp \
        gestorcatalogo.cpp \
        gestorusuarios.cpp \
        listafavoritos.cpp \
        main.cpp \
        reproductor.cpp \
        sistemaudeatunes.cpp \
        usuario.cpp

HEADERS += \
    Album.h \
    Artista.h \
    MensajePublicitario.h \
    cancion.h \
    gestorcatalogo.h \
    gestorusuarios.h \
    listafavoritos.h \
    reproductor.h \
    sistemaudeatunes.h \
    usuario.h
