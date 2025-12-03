//
// Created by aleji on 2/12/2025.
//

// Tablero.cpp
#include "Tablero.h"
#include <iostream>
#include <cstdlib>  // rand
using namespace std;

Tablero::Tablero(int tamano, Jugador** jugadores, int numJugadores) {
    this->tamano = tamano;
    this->numJugadores = numJugadores;
    this->lugarJugador = jugadores;
    numEspeciales = 0;

    // Reservar matriz [tamano][tamano] de punteros a Casilla
    tablero = new Casilla**[tamano];
    for (int i = 0; i < tamano; i++) {
        tablero[i] = new Casilla*[tamano];
        for (int j = 0; j < tamano; j++) {
            tablero[i][j] = new CasillaNormal(i, j);  // todo normal al inicio
        }
    }
}

int Tablero::getTamano() {
    return tamano;
}

Casilla* Tablero::getCasilla(int fila, int columna) {
    if (!validarPosicion(fila, columna)) {
        return nullptr;
    }
    return tablero[fila][columna];
}

bool Tablero::validarPosicion(int fila, int columna) {
    return fila >= 0 && fila < tamano &&
           columna >= 0 && columna < tamano;
}

bool Tablero::estaEnLimite(int fila, int columna) {
    return fila == 0 || fila == tamano - 1 ||
           columna == 0 || columna == tamano - 1;
}

bool Tablero::estaEnEsquina(int fila, int columna) {
    bool arribaIzq  = (fila == 0 && columna == 0);
    bool arribaDer  = (fila == 0 && columna == tamano - 1);
    bool abajoDer   = (fila == tamano - 1 && columna == tamano - 1);
    bool abajoIzq   = (fila == tamano - 1 && columna == 0);
    return arribaIzq || arribaDer || abajoDer || abajoIzq;
}

bool Tablero::meta(int fila, int columna) {
    // Centro del tablero (tamaños 15, 23, 31 → índice tamano/2) [file:2]
    int centro = tamano / 2;
    return fila == centro && columna == centro;
}

void Tablero::generarEspeciales(int porcentaje) {
    // porcentaje recomendado 6–10 según el enunciado [file:2]
    int totalCasillas = tamano * tamano;
    int cantidad = totalCasillas * porcentaje / 100;

    numEspeciales = 0;
    while (numEspeciales < cantidad) {
        int fila = rand() % tamano;
        int col  = rand() % tamano;

        // si ya es especial, saltar
        if (dynamic_cast<CasillaEspecial*>(tablero[fila][col]) != nullptr) {
            continue;
        }

        // opcional: evitar marcar la casilla meta como especial
        if (meta(fila, col)) {
            continue;
        }

        // Cambiar la casilla normal por una especial
        delete tablero[fila][col];
        tablero[fila][col] = new CasillaEspecial(fila, col);
        numEspeciales++;
    }
}

void Tablero::colocarJugadores() {
    // Coloca hasta 4 jugadores en las esquinas como dice el pdf [file:2]
    if (numJugadores > 0 && lugarJugador[0] != nullptr) {
        lugarJugador[0]->setPosicion(0, 0);                     // arriba izquierda
    }
    if (numJugadores > 1 && lugarJugador[1] != nullptr) {
        lugarJugador[1]->setPosicion(0, tamano - 1);            // arriba derecha
    }
    if (numJugadores > 2 && lugarJugador[2] != nullptr) {
        lugarJugador[2]->setPosicion(tamano - 1, tamano - 1);   // abajo derecha
    }
    if (numJugadores > 3 && lugarJugador[3] != nullptr) {
        lugarJugador[3]->setPosicion(tamano - 1, 0);            // abajo izquierda
    }
}

void Tablero::dibujarTablero() {
    // Versión de texto: N = normal, E = especial
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            if (dynamic_cast<CasillaEspecial*>(tablero[i][j]) != nullptr) {
                cout << "E ";
            } else {
                cout << "N ";
            }
        }
        cout << endl;
    }
}
