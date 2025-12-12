//
// Created by aleji on 2/12/2025.
//
// Tablero.cpp
#include "Tablero.h"
#include <iostream>
   // rand
#include <ctime>     // time

using namespace std;

Tablero::Tablero(int tamano, Jugador** jugadores, int numJugadores) {
    this->tamano = tamano;
    this->numJugadores = numJugadores;
    this->lugarJugador = jugadores;
    numEspeciales = 0;

    // inicializar punto de inicio de rand
    srand(time(nullptr));

    // crear matriz [tamano][tamano] de punteros a Casilla (creacion de casillas en tablero)
    tablero = new Casilla**[tamano];
    for (int i = 0; i < tamano; i++) {
        tablero[i] = new Casilla*[tamano];
        for (int j = 0; j < tamano; j++) {
            tablero[i][j] = new CasillaNormal(i, j);
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

//Verifica una posicion válida
bool Tablero::validarPosicion(int fila, int columna) {
    return fila >= 0 && fila < tamano && columna >= 0 && columna < tamano;
}

//verifica los limites de el tablero
bool Tablero::estaEnLimite(int fila, int columna) {
    return fila == 0 || fila == tamano - 1 ||columna == 0 || columna == tamano - 1;
}
//verifica las esquinas
bool Tablero::estaEnEsquina(int fila, int columna) {
    bool arribaIzq  = (fila == 0 && columna == 0);
    bool arribaDer  = (fila == 0 && columna == tamano - 1);
    bool abajoDer   = (fila == tamano - 1 && columna == tamano - 1);
    bool abajoIzq   = (fila == tamano - 1 && columna == 0);
    return arribaIzq || arribaDer || abajoDer || abajoIzq;
}

bool Tablero::meta(int fila, int columna) {
    // Centro del tablero (tamaños 15, 23, 31 -> índice tamano/2)
    int centro = tamano / 2;
    return fila == centro && columna == centro;
}

void Tablero::generarEspeciales(int porcentaje) {
    // porcentaje recomendado 6–10 según el enunciado
    int totalCasillas = tamano * tamano;
    int cantidad = totalCasillas * porcentaje / 100;

    numEspeciales = 0;
    while (numEspeciales < cantidad) {
        int fila = rand() % tamano;
        int col  = rand() % tamano;

        // evitar esquinas y meta
        if (estaEnEsquina(fila, col) || meta(fila, col)) {
            continue;
        }

        // si ya es especial, saltar
        CasillaEspecial* existeEspecial =
                dynamic_cast<CasillaEspecial*>(tablero[fila][col]);
        if (existeEspecial != nullptr) {
            continue;
        }

        // Cambiar la casilla normal por una especial
        delete tablero[fila][col];
        tablero[fila][col] = new CasillaEspecial(fila, col);
        numEspeciales++;
    }
}

void Tablero::colocarJugadores() {
    // Lista de las 4 esquinas
    int esquinasFila[4]    = {0, 0, tamano - 1, tamano - 1};
    int esquinasColumna[4] = {0, tamano - 1, tamano - 1, 0};

    // Desordenar las esquinas de forma aleatoria  (esto es un codigo para poner
    // los jugadores en esquinas aleatorias)
    for (int i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmpF = esquinasFila[i];
        int tmpC = esquinasColumna[i];
        esquinasFila[i]    = esquinasFila[j];
        esquinasColumna[i] = esquinasColumna[j];
        esquinasFila[j]    = tmpF;
        esquinasColumna[j] = tmpC;
    }

    // Asignar una esquina distinta a cada jugador existente
    int limite = numJugadores;
    if (limite > 4) {
        limite = 4;
    }

    for (int i = 0; i < limite; i++) {
        if (lugarJugador[i] != nullptr) {
            int f = esquinasFila[i];
            int c = esquinasColumna[i];
            lugarJugador[i]->setPosicion(f, c);
        }
    }
}
// valida una posicion valida del tablero
void Tablero::setCasilla(int fila, int columna, Casilla* nuevaCasilla) {
    if (!validarPosicion(fila, columna)) return;
    delete tablero[fila][columna];
    tablero[fila][columna] = nuevaCasilla;
}

void Tablero::dibujarTablero() {
    // matriz temporal vacía, sin meta, especiales ni nada
    char** vista = new char*[tamano];
    for (int i = 0; i < tamano; i++) {
        vista[i] = new char[tamano];
        for (int j = 0; j < tamano; j++) {
            vista[i][j] = ' '; // base vacía
        }
    }

    // Marcar meta
    int centro = tamano / 2;
    vista[centro][centro] = 'M';

    // Casillas especiales
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            // dynamic_cast se usa en proyectos con herencia y polimorfismo
            // esto valida el tipo de casilla sin generar cambios al proyecto
            //esto evita crasheos y permite un correcto funcionamiento de casillanormal y especial
            CasillaEspecial* ce = dynamic_cast<CasillaEspecial*>(tablero[i][j]);
            if (ce != nullptr) {
                if (!ce->getConocida()) {
                    vista[i][j] = '?';
                } else {
                    if (ce->getTipo() < 0) {
                        vista[i][j] = 'C'; // castigo
                    } else if (ce->getTipo() > 0) {
                        vista[i][j] = 'R'; // recompensa
                    } else {
                        vista[i][j] = '?';
                    }
                }
            }
        }
    }

    // Colocar jugadores (1..4), sobreescribiendo lo que hubiera debajo
    int limite = numJugadores;
    if (limite > 4) {
        limite = 4;
    }

    for (int i = 0; i < limite; i++) {
        if (lugarJugador[i] != nullptr && !lugarJugador[i]->estaEliminado()) {
            int f = lugarJugador[i]->getFila();
            int c = lugarJugador[i]->getColumna();
            if (validarPosicion(f, c)) {
                char simbolo = '1' + i; // '1','2','3','4'
                vista[f][c] = simbolo;//recordemos que vista era la matriz inicial vacía
                //esto la va llenando con los caracteres en la posición
            }
        }
    }

    // vista del tablero
    cout << "\nTablero:\n";
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            cout << "[" << vista[i][j] << "]";
        }
        cout << endl;
    }

    for (int i = 0; i < tamano; i++) {
        delete[] vista[i];
    }
    delete[] vista;
}
