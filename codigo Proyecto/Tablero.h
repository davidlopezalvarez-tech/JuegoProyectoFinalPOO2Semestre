//
// Created by aleji on 2/12/2025.
//

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
    Jugador** lugarJugador;  // arreglo de punteros a jugadores
    int numJugadores;

public:
    Tablero(int tamano, Jugador** jugadores, int numJugadores);

    int getTamano();
    Casilla* getCasilla(int fila, int columna);

    void dibujarTablero();                    // muestra vacias, especiales, meta y jugadores
    void generarEspeciales(int porcentaje);   // 6–10% de especiales, sin esquinas ni meta
    void colocarJugadores();                  // jugadores en esquinas aleatorias

    bool validarPosicion(int fila, int columna);
    bool estaEnLimite(int fila, int columna);
    bool estaEnEsquina(int fila, int columna);
    bool meta(int fila, int columna);         // casilla central

    // NUEVO: para que GestorPartida pueda reconstruir casillas al cargar
    void setCasilla(int fila, int columna, Casilla* nuevaCasilla);
};

#endif //PROYECTO_FINAL_TABLERO_H
