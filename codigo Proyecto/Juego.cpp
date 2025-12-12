// Juego.cpp
#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(int dificultad, int numJugadores) {
    this->dificultad = dificultad;

    if (numJugadores < 1) numJugadores = 1;  //min jugadoras
    if (numJugadores > 4) numJugadores = 4; //max jugadores
    this->numJugadores = numJugadores;

    jugadores = new Jugador*[this->numJugadores];

    // Tamaño de tablero y vida inicial segun dificultad.
    if (dificultad == 1) {        // facil
        tamanoTablero = 15;
        pvInicial = 15;
    } else if (dificultad == 2) { // medio
        tamanoTablero = 23;
        pvInicial = 23;
    } else {                      // dificil
        tamanoTablero = 31;
        pvInicial = 31;
    }

    // Crear jugadores con PV inicial; posicion real se fija en Tablero.
    for (int i = 0; i < this->numJugadores; i++) {
        jugadores[i] = new Jugador(i + 1, pvInicial, 0, 0);
    }
                                                    //agrega el numero de jugadores
    tablero = new Tablero(tamanoTablero, jugadores, this->numJugadores);
    jugadorActual = 0;
}

int Juego::getDificultad() {
    return dificultad;
}
int Juego::getPVInicial() {
    return pvInicial;
}
int Juego::getNumJugadores() {
    return numJugadores;
}
int Juego::getJugadorActual() {
    return jugadorActual;
}
Jugador** Juego::getJugadores() {
    return jugadores;
}
Tablero* Juego::getTablero() {
    return tablero;
}

void Juego::iniciarPartida() {
    // 8% de casillas especiales (rango 6–10%).
    tablero->generarEspeciales(8);
    tablero->colocarJugadores();

    cout << "Tablero creado de tamano " << tablero->getTamano()
         << ", PV inicial " << pvInicial
         << ", jugadores: " << numJugadores << endl;

    tablero->dibujarTablero();
    mostrarEstado();
}

void Juego::mostrarEstado() {
    cout << "\nEstado de los jugadores:\n";
    for (int i = 0; i < numJugadores; i++) {
        int fila = jugadores[i]->getFila();
        int col  = jugadores[i]->getColumna();
        cout << "Jugador " << jugadores[i]->getId()
             << " -> PV: " << jugadores[i]->getVidaActual()
             << " Posicion: (" << fila << ", " << col << ")";
        if (jugadores[i]->estaEliminado()) {
            cout << " [ELIMINADO]";
        }
        cout << "\n";
    }
}


// Mueve una sola casilla en la direccion indicada y descuenta 1 PV.
bool Juego::moverJugadorUnaCasilla(int indiceJugador, int dirFila, int dirCol) {
    Jugador* j = jugadores[indiceJugador];

    int fila = j->getFila();
    int col  = j->getColumna();

    int nuevaFila = fila + dirFila;
    int nuevaCol  = col  + dirCol;

    if (!tablero->validarPosicion(nuevaFila, nuevaCol)) {
        cout << "Movimiento cancelado: se saldria del tablero.\n";
        return false;
    }

    j->setPosicion(nuevaFila, nuevaCol);

    int vida = j->getVidaActual();
    j->setVida(vida - 1);   // un paso, un PV

    if (j->estaEliminado()) {
        cout << "Jugador " << j->getId()
             << " ha sido eliminado por llegar a 0 PV.\n";
        return false;
    }

    cout << "Jugador " << j->getId()
         << " se movio a (" << nuevaFila << ", " << nuevaCol << ")\n";

    // Si llega exactamente a la casilla central, gana.
    if (tablero->meta(nuevaFila, nuevaCol)) {
        cout << "Jugador " << j->getId()
             << " ha llegado a la meta y gana la partida.\n";
        // Aqui luego podrias marcar fin de juego global.
    }

    // Aplicar efecto de casilla especial si corresponde.
    aplicarCasillaEspecial(indiceJugador);

    return true;
}

// Aplicar reglas de casillas especiales (castigo / recompensa).
void Juego::aplicarCasillaEspecial(int indiceJugador) {
    Jugador* j = jugadores[indiceJugador];
    int fila = j->getFila();
    int col  = j->getColumna();

    Casilla* c = tablero->getCasilla(fila, col);
    CasillaEspecial* ce = dynamic_cast<CasillaEspecial*>(c);

    if (ce == nullptr) {
        return;
    }

    cout << "Jugador " << j->getId() << " ha caido en una casilla especial.\n";

    // Limite del dado: max(pvInicial, vidaActual), minimo 1
    int vidaActual = j->getVidaActual();
    int limiteDado = (vidaActual > pvInicial) ? vidaActual : pvInicial;
    if (limiteDado < 1) limiteDado = 1;
    int d = dado.lanzar(limiteDado);
    cout << "Valor del dado especial: " << d << endl;

    // Si aun no se conocia, decidir al azar castigo o recompensa.
    if (!ce->getConocida()) {
        int tipoRandom = dado.lanzar(2); // 1 o 2
        if (tipoRandom == 1) {
            ce->setTipo(-1); // negativo = castigo
        } else {
            ce->setTipo(1);  // positivo = recompensa
        }
        ce->setConocida(true);
    }

    if (ce->getTipo() < 0) {
        // Castigo: restar d PV al que cayo.
        ce->castigar(j, d);
        cout << "Casilla de castigo: -" << d
             << " PV al jugador " << j->getId() << endl;
    } else {
        // Recompensa: elegir opcion.
        int opcion;
        cout << "Casilla de recompensa:\n";
        cout << "1) Restar " << d << " PV a los demas jugadores\n";
        cout << "2) Sumarse " << d << " PV a si mismo\n";
        cout << "Elige opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            for (int i = 0; i < numJugadores; i++) {
                if (i == indiceJugador) continue;
                if (jugadores[i]->estaEliminado()) continue;
                ce->castigar(jugadores[i], d);
            }
        } else {
            ce->recompensa(j, d);
        }
    }
}

// Turno completo: direcciones posibles por tablero, validas si PV > dado y dentro.
void Juego::turnoJugador() {
    Jugador* j = jugadores[jugadorActual];

    if (j->estaEliminado()) {
        cout << "Jugador " << j->getId()
             << " esta eliminado. Se salta su turno.\n";
        jugadorActual = jugadorActual + 1;
        if (jugadorActual >= numJugadores) {
            jugadorActual = 0;
        }
        return;
    }


    int fila = j->getFila();
    int col  = j->getColumna();

    cout << "\n--- Turno del jugador " << j->getId() << " ---\n";
    cout << "PV actuales: " << j->getVidaActual()
         << " Posicion: (" << fila << ", " << col << ")\n";

    // Direcciones: 0=Arriba,1=Abajo,2=Izquierda,3=Derecha.
    int dirFila[4]        = {-1, 1, 0, 0};
    int dirCol[4]         = {0, 0, -1, 1};
    const char* nombre[4] = {"Arriba", "Abajo", "Izquierda", "Derecha"};

    int valores[4]          = {0, 0, 0, 0};
    bool direccionDentro[4] = {false, false, false, false};
    bool valida[4]          = {false, false, false, false};

    // Primero: ver qué direcciones NO se salen (1 casilla).
    for (int i = 0; i < 4; i++) {
        int nuevaFila = fila + dirFila[i];
        int nuevaCol  = col  + dirCol[i];
        direccionDentro[i] = tablero->validarPosicion(nuevaFila, nuevaCol);
    }

    // Lanzar dados solo para direcciones que están dentro.
    cout << "Se lanzan dados para las direcciones posibles.\n";
    for (int i = 0; i < 4; i++) {
        if (!direccionDentro[i]) {
            continue; // ni siquiera se considera esta direccion
        }

        int vidaActual = j->getVidaActual();
        int limiteDado = (vidaActual > pvInicial) ? vidaActual : pvInicial;
        if (limiteDado < 1) limiteDado = 1;

        valores[i] = dado.lanzar(limiteDado);

        // Tu regla: direccion valida si PV actual > valor del dado
        bool reglaPV = (j->getVidaActual() > valores[i]);

        if (reglaPV) {
            valida[i] = true;
        } else {
            valida[i] = false;
        }
    }

    cout << "Resultados de los dados y validez de direcciones:\n";
    for (int i = 0; i < 4; i++) {
        if (!direccionDentro[i]) {
            cout << i << ") " << nombre[i]
                 << " -> NO es posible moverse en esta direccion (fuera del tablero).\n";
            continue;
        }

        cout << i << ") " << nombre[i]
             << " - dado = " << valores[i] << " -> ";
        if (valida[i]) {
            cout << "MOVIMIENTO VALIDO\n";
        } else {
            cout << "NO es posible moverse en esta direccion (PV <= dado).\n";
        }
    }

    // Contar direcciones validas.
    int cuentaValidas = 0;
    int ultimaValida = -1;
    for (int i = 0; i < 4; i++) {
        if (valida[i]) {
            cuentaValidas++;
            ultimaValida = i;
        }
    }

    if (cuentaValidas == 0) {
        cout << "No hay direcciones validas. El jugador pierde el turno.\n";
        jugadorActual = jugadorActual + 1;
        if (jugadorActual >= numJugadores) {
            jugadorActual = 0;
        }
        return;
    }


    int eleccion = ultimaValida;

    if (cuentaValidas > 1) {
        cout << "Elige una direccion valida:\n";
        for (int i = 0; i < 4; i++) {
            if (valida[i]) {
                cout << i << ") " << nombre[i]
                     << " (dado = " << valores[i] << ")\n";
            }
        }
        cout << "Opcion: ";
        cin >> eleccion;
        if (eleccion < 0 || eleccion >= 4 || !valida[eleccion]) {
            cout << "Opcion invalida, se anula el turno.\n";
            jugadorActual = jugadorActual + 1;
            if (jugadorActual >= numJugadores) {
                jugadorActual = 0;
            }
            return;
        }

    } else {
        cout << "Solo hay una direccion valida: "
             << nombre[ultimaValida]
             << " (dado = " << valores[ultimaValida]
             << "). Movimiento automatico.\n";
    }

    // Mover siempre UNA casilla en la direccion elegida.
    moverJugadorUnaCasilla(jugadorActual,
                           dirFila[eleccion], dirCol[eleccion]);

    // Mostrar tablero y estado despues del movimiento.
    tablero->dibujarTablero();
    mostrarEstado();

    // Pasar al siguiente jugador
    jugadorActual = jugadorActual + 1;
    if (jugadorActual >= numJugadores) {
        jugadorActual = 0;
    }

}
