TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        album.cpp \
        artista.cpp \
        cancion.cpp \
        creditos.cpp \
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
    creditos.h \
    gestorusuarios.h \
    listafavoritos.h \
    mensajepublicitario.h \
    reproductor.h \
    sistemaudeatunes.h \
    usuario.h
