#include "Jugador.h"

Jugador::Jugador() {
    id = 0;
    maxVida = 0;
    vidaActual = 0;
    eliminado = false;
    filaJugador = 0;
    columnaJugador = 0;
}

Jugador::Jugador(int id, int maxVidaInicial, int filaInicial, int colInicial) {
    this->id = id;
    maxVida = maxVidaInicial;
    vidaActual = maxVidaInicial;
    eliminado = false;
    filaJugador = filaInicial;
    columnaJugador = colInicial;
}

int Jugador::getId() {
    return id;
}
int Jugador::getMaxVida() {
    return maxVida;
}
int Jugador::getVidaActual() {
    return vidaActual;
}
bool Jugador::estaEliminado() {
    return eliminado;
}
int Jugador::getFila() {
    return filaJugador;
}
int Jugador::getColumna() {
    return columnaJugador;
}

void Jugador::setVida(int vidaActual) {
    this->vidaActual = vidaActual;
    if (this->vidaActual <= 0) {
        this->vidaActual = 0;
        eliminado = true;
    }
}

void Jugador::setPosicion(int fila, int columna) {
    filaJugador = fila;
    columnaJugador = columna;
}