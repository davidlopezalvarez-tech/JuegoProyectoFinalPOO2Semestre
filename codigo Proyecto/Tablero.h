//
// Created by aleji on 2/12/2025.
//
// Tablero.h
#ifndef PROYECTO_FINAL_TABLERO_H
#define PROYECTO_FINAL_TABLERO_H

#include "Casilla.h"
#include "CasillaNormal.h"
#include "CasillaEspecial.h"
#include "Jugador.h"

class Tablero {
private:
    int tamano;
    int numEspeciales;
    Casilla*** tablero;      // matriz [fila][columna] de punteros a Casilla
    Jugador** lugarJugador;  // verifica el lugar del jugador (fila, columna)
    int numJugadores;

public:
    Tablero(int tamano, Jugador** jugadores, int numJugadores);

    int getTamano();
    Casilla* getCasilla(int fila, int columna);

    void dibujarTablero();
    void generarEspeciales(int porcentaje); // para el porcentaje de 6 a 10% de especiales
    void colocarJugadores();             // pone los jugadores en las 4 esquinas
    bool validarPosicion(int fila, int columna);
    bool estaEnLimite(int fila, int columna);
    bool estaEnEsquina(int fila, int columna);
    bool meta(int fila, int columna);
};

#endif //PROYECTO_FINAL_TABLERO_H
