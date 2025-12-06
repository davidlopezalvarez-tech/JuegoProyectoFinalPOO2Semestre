//
// Created by aleji on 6/12/2025.
//
// GestorPartida.h
#ifndef PROYECTO_FINAL_GESTORPARTIDA_H
#define PROYECTO_FINAL_GESTORPARTIDA_H

#include "Juego.h"

class GestorPartida {
public:
    void mostrarPartidas();                         // muestra estado de partida1..3
    bool guardarPartida(int numeroPartida, Juego* juego);
    bool cargarPartida(int numeroPartida, Juego*& juego);
    bool eliminarPartida(int numeroPartida);
};

#endif //PROYECTO_FINAL_GESTORPARTIDA_H
