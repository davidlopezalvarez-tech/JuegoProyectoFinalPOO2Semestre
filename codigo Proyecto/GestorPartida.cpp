// GestorPartida.cpp
#include "GestorPartida.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>   // remove
using namespace std;

// nombre de archivo segun numero de partida (1..3)
static string nombreArchivoPartida(int numeroPartida) {
    if (numeroPartida == 1) return "partida1.txt";
    if (numeroPartida == 2) return "partida2.txt";
    if (numeroPartida == 3) return "partida3.txt";
    return "";
}

void GestorPartida::mostrarPartidas() {
    cout << "\n=== Partidas guardadas ===\n";
    for (int i = 1; i <= 3; i++) {
        string nombre = nombreArchivoPartida(i);
        if (nombre == "") continue;

        ifstream archivo(nombre.c_str());
        if (archivo.good()) {
            cout << i << ") " << nombre << " -> OCUPADA\n";
        } else {
            cout << i << ") " << nombre << " -> VACIA\n";
        }
        archivo.close();
    }
}

bool GestorPartida::guardarPartida(int numeroPartida, Juego* juego) {
    string nombre = nombreArchivoPartida(numeroPartida);
    if (nombre == "" || juego == nullptr) {
        return false;
    }

    ofstream out(nombre.c_str());
    if (!out.is_open()) {
        return false;
    }

    int dificultad    = juego->getDificultad();
    int pvInicial     = juego->getPVInicial();
    int numJugadores  = juego->getNumJugadores();
    int jugadorActual = juego->getJugadorActual();
    Tablero* tablero  = juego->getTablero();
    int tamano        = tablero->getTamano();
    Jugador** jugadores = juego->getJugadores();

    // Cabecera
    out << dificultad    << " "
        << pvInicial     << " "
        << numJugadores  << " "
        << jugadorActual << " "
        << tamano        << "\n";

    // Jugadores
    for (int i = 0; i < numJugadores; i++) {
        Jugador* j = jugadores[i];
        out << j->getId()          << " "
            << j->getMaxVida()     << " "
            << j->getVidaActual()  << " "
            << j->estaEliminado()  << " "
            << j->getFila()        << " "
            << j->getColumna()     << "\n";
    }

    // Tablero: codigos por casilla
    // 0 = normal
    // 1 = especial desconocida
    // 2 = especial castigo conocida
    // 3 = especial recompensa conocida
    for (int i = 0; i < tamano; i++) {
        for (int j = 0; j < tamano; j++) {
            Casilla* c = tablero->getCasilla(i, j);
            CasillaEspecial* ce = dynamic_cast<CasillaEspecial*>(c);

            int codigo = 0;
            if (ce != nullptr) {
                if (!ce->getConocida()) {
                    codigo = 1;
                } else {
                    if (ce->getTipo() < 0) {
                        codigo = 2;
                    } else if (ce->getTipo() > 0) {
                        codigo = 3;
                    } else {
                        codigo = 1;
                    }
                }
            }
            out << codigo;
            if (j < tamano - 1) out << " ";
        }
        out << "\n";
    }

    out.close();
    return true;
}

bool GestorPartida::cargarPartida(int numeroPartida, Juego*& juego) {
    string nombre = nombreArchivoPartida(numeroPartida);
    if (nombre == "") {
        return false;
    }

    ifstream in(nombre.c_str());
    if (!in.is_open()) {
        return false;
    }

    int dificultad;
    int pvInicialArchivo;
    int numJugadoresArchivo;
    int jugadorActualArchivo;
    int tamanoArchivo;

    // Leer cabecera
    if (!(in >> dificultad >> pvInicialArchivo
            >> numJugadoresArchivo >> jugadorActualArchivo
            >> tamanoArchivo)) {
        in.close();
        return false;
    }

    if (numJugadoresArchivo < 1) numJugadoresArchivo = 1;
    if (numJugadoresArchivo > 4) numJugadoresArchivo = 4;

    // Crear nuevo juego con los datos basicos
    Juego* nuevoJuego = new Juego(dificultad, numJugadoresArchivo);
    Jugador** jugadores = nuevoJuego->getJugadores();
    Tablero* tablero    = nuevoJuego->getTablero();
    int tamanoNuevo     = tablero->getTamano();

    // Verificar que el tamaño coincide
    if (tamanoNuevo != tamanoArchivo) {
        in.close();
        delete nuevoJuego;
        return false;
    }

    // Leer jugadores y sobreescribir datos basicos
    for (int i = 0; i < numJugadoresArchivo; i++) {
        int id, maxVida, vidaActual, eliminadoInt, fila, columna;
        if (!(in >> id >> maxVida >> vidaActual >> eliminadoInt >> fila >> columna)) {
            in.close();
            delete nuevoJuego;
            return false;
        }
        Jugador* j = jugadores[i];
        j->setPosicion(fila, columna);
        j->setVida(vidaActual);
    }

    // Leer matriz de casillas y reconstruir
    for (int i = 0; i < tamanoArchivo; i++) {
        for (int j = 0; j < tamanoArchivo; j++) {
            int codigo;
            if (!(in >> codigo)) {
                in.close();
                delete nuevoJuego;
                return false;
            }

            if (codigo == 0) {
                tablero->setCasilla(i, j, new CasillaNormal(i, j));
            } else {
                CasillaEspecial* ce = new CasillaEspecial(i, j);
                if (codigo == 1) {         // desconocida
                    ce->setConocida(false);
                    ce->setTipo(0);
                } else if (codigo == 2) {  // castigo conocida
                    ce->setConocida(true);
                    ce->setTipo(-1);
                } else if (codigo == 3) {  // recompensa conocida
                    ce->setConocida(true);
                    ce->setTipo(1);
                }
                tablero->setCasilla(i, j, ce);
            }
        }
    }

    in.close();

    // Reemplazar juego actual
    delete juego;
    juego = nuevoJuego;

    return true;
}

bool GestorPartida::eliminarPartida(int numeroPartida) {
    string nombre = nombreArchivoPartida(numeroPartida);
    if (nombre == "") {
        return false;
    }

    if (std::remove(nombre.c_str()) == 0) {
        return true;
    } else {
        return false;
    }
}
