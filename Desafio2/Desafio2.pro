TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        album.cpp \
        artista.cpp \
        cancion.cpp \
        gestorcatalogo.cpp \
        gestorusuarios.cpp \
        listafavoritos.cpp \
        main.cpp \
        mensajepublicitario.cpp \
        reproductor.cpp \
        sistemaudeatunes.cpp \
        usuario.cpp

HEADERS += \
    album.h \
    artista.h \
    cancion.h \
    gestorcatalogo.h \
    gestorusuarios.h \
    listafavoritos.h \
    mensajepublicitario.h \
    reproductor.h \
    sistemaudeatunes.h \
    usuario.h
