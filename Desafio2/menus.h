#ifndef MENUS_H
#define MENUS_H

#include "SistemaUdeATunes.h"
#include "listafavoritos.h"
#include <iostream>
#include <string>
#include <limits>

void limpiarBuffer();
void pausarPantalla();
void mostrarEncabezado(const std::string& titulo);

void mostrarMenuPrincipal();
void mostrarMenuUsuario(SistemaUdeATunes& sistema);
void menuFavoritos(SistemaUdeATunes& sistema);

#endif
