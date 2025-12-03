//
// Created by aleji on 2/12/2025.
//
#include "Dado.h"

#include <cstdlib>  // variable para random
#include <ctime>  // variable auxiliar para random, así funciona bien el random

Dado::Dado() {
    srand(time(nullptr)); // inicializar la semilla
}

int Dado::lanzar(int maximo) {
    if (maximo <= 0) {
        return 1; // por seguridad
    }
    int valor = (rand() % maximo) + 1; // rango 1..maximo
    return valor;
}
