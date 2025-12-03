// Juego.h
#ifndef PROYECTO_FINAL_JUEGO_H
#define PROYECTO_FINAL_JUEGO_H

#include "Tablero.h"
#include "Dado.h"
#include "Jugador.h"

class Juego {
private:
    int dificultad;      // 1 = Facil, 2 = Medio, 3 = Dificil
    int tamanoTablero;   // 15, 23 o 31
    int pvInicial;       // PV inicial por jugador, segun dificultad

    Jugador** jugadores; // arreglo de 4 punteros a Jugador
    int numJugadores;

    Tablero* tablero;
    Dado dado;

    int jugadorActual;   // indice 0..3

    int calcularNumDados(int fila, int col);
    bool moverJugador(int indiceJugador, int dirFila, int dirCol, int pasos);
    void aplicarCasillaEspecial(int indiceJugador);

public:
    Juego(int dificultad);

    void iniciarPartida();
    void mostrarEstado();
    void turnoJugador();   // ejecuta un turno completo del jugadorActual
};

#endif //PROYECTO_FINAL_JUEGO_H
