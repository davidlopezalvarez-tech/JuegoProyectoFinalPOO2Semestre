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

    Jugador** jugadores; // arreglo de punteros a Jugador
    int numJugadores;    // 1..4

    Tablero* tablero;
    Dado dado;

    int jugadorActual;   // indice 0..numJugadores-1

    int calcularNumDados(int fila, int col);
    bool moverJugadorUnaCasilla(int indiceJugador, int dirFila, int dirCol);
    void aplicarCasillaEspecial(int indiceJugador);

public:
    Juego(int dificultad, int numJugadores);

    void iniciarPartida();
    void mostrarEstado();
    void turnoJugador();   // ejecuta un turno completo del jugadorActual

    // getters simples que luego usara GestorPartida
    int getDificultad();
    int getPVInicial();
    int getNumJugadores();
    int getJugadorActual();
    Jugador** getJugadores();
    Tablero* getTablero();
};

#endif //PROYECTO_FINAL_JUEGO_H
