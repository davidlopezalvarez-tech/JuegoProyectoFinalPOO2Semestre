// Juego.cpp
#include "Juego.h"
#include <iostream>
using namespace std;

Juego::Juego(int dificultad) {
    this->dificultad = dificultad;
    numJugadores = 4;
    jugadores = new Jugador*[numJugadores];

    // Elegir tamaño de tablero y PV inicial segun el pdf. [file:2]
    if (dificultad == 1) {        // Facil
        tamanoTablero = 15;
        pvInicial = 15;
    } else if (dificultad == 2) { // Medio
        tamanoTablero = 23;
        pvInicial = 23;
    } else {                      // Dificil
        tamanoTablero = 31;
        pvInicial = 31;
    }

    // Crear 4 jugadores con PV inicial; la posicion real se asigna en Tablero. [file:2][file:63]
    for (int i = 0; i < numJugadores; i++) {
        jugadores[i] = new Jugador(i + 1, pvInicial, 0, 0);
    }

    // Crear el tablero y asociarle los jugadores.
    tablero = new Tablero(tamanoTablero, jugadores, numJugadores);
    jugadorActual = 0;
}

void Juego::iniciarPartida() {
    // Generar un ~8% de casillas especiales (rango sugerido 6–10%). [file:2]
    tablero->generarEspeciales(8);
    // Colocar a los 4 jugadores en las esquinas. [file:2]
    tablero->colocarJugadores();

    cout << "Tablero creado de tamano " << tablero->getTamano()
         << ", PV inicial " << pvInicial << endl;

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
             << " Posicion: (" << fila << ", " << col << ")\n";
    }
}

// Esquina: 2 dados; borde: 3; interior: 4, segun el pdf. [file:2]
int Juego::calcularNumDados(int fila, int col) {
    if (tablero->estaEnEsquina(fila, col)) {
        return 2;
    } else if (tablero->estaEnLimite(fila, col)) {
        return 3;
    } else {
        return 4;
    }
}

// Mueve al jugador paso a paso, restando 1 PV por casilla recorrida. [file:2]
bool Juego::moverJugador(int indiceJugador, int dirFila, int dirCol, int pasos) {
    Jugador* j = jugadores[indiceJugador];

    int fila = j->getFila();
    int col  = j->getColumna();

    for (int k = 0; k < pasos; k++) {
        int nuevaFila = fila + dirFila;
        int nuevaCol  = col  + dirCol;

        if (!tablero->validarPosicion(nuevaFila, nuevaCol)) {
            // No se permite salir del tablero. [file:2]
            cout << "Movimiento detenido: se saldria del tablero.\n";
            return false;
        }

        fila = nuevaFila;
        col  = nuevaCol;
        j->setPosicion(fila, col);

        // Cada casilla recorrida cuesta 1 PV. [file:2]
        int vida = j->getVidaActual();
        j->setVida(vida - 1);

        if (j->estaEliminado()) {
            cout << "Jugador " << j->getId()
                 << " ha sido eliminado por llegar a 0 PV.\n";
            return false;
        }
    }

    cout << "Jugador " << j->getId()
         << " se movio a (" << fila << ", " << col << ")\n";

    // Si llega exactamente a la casilla central, gana. [file:2]
    if (tablero->meta(fila, col)) {
        cout << "Jugador " << j->getId()
             << " ha llegado a la meta y gana la partida.\n";
        // Aqui luego podrias marcar fin de juego.
    }

    // Aplicar efecto de casilla especial si corresponde. [file:2]
    aplicarCasillaEspecial(indiceJugador);

    return true;
}

// Aplica las reglas de casillas especiales (castigo o recompensa). [file:2]
void Juego::aplicarCasillaEspecial(int indiceJugador) {
    Jugador* j = jugadores[indiceJugador];
    int fila = j->getFila();
    int col  = j->getColumna();

    Casilla* c = tablero->getCasilla(fila, col);
    CasillaEspecial* ce = dynamic_cast<CasillaEspecial*>(c);

    if (ce == nullptr) {
        return; // No es casilla especial.
    }

    cout << "Jugador " << j->getId() << " ha caido en una casilla especial.\n";

    int d = dado.lanzar(pvInicial); // valor entre 1 y PV inicial. [file:2][file:66]
    cout << "Valor del dado especial: " << d << endl;

    // Si la casilla aun no se conocia, decidir al azar si es castigo o recompensa. [file:2]
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
        // Castigo: restar d PV al jugador que cayo. [file:2]
        ce->castigar(j, d);
        cout << "Casilla de castigo: -" << d
             << " PV al jugador " << j->getId() << endl;
    } else {
        // Recompensa: elegir entre dañar a los demas o curarse. [file:2]
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

// Un turno completo del jugadorActual, siguiendo reglas 5.1 y 5.2. [file:2]
void Juego::turnoJugador() {
    Jugador* j = jugadores[jugadorActual];

    if (j->estaEliminado()) {
        cout << "Jugador " << j->getId()
             << " esta eliminado. Se salta su turno.\n";
        jugadorActual = (jugadorActual + 1) % numJugadores;
        return;
    }

    int fila = j->getFila();
    int col  = j->getColumna();

    cout << "\n--- Turno del jugador " << j->getId() << " ---\n";
    cout << "PV actuales: " << j->getVidaActual()
         << " Posicion: (" << fila << ", " << col << ")\n";

    int numDados = calcularNumDados(fila, col);
    cout << "Se lanzan " << numDados << " dados.\n";

    // Direcciones en orden: 0=Arriba,1=Abajo,2=Izquierda,3=Derecha. [file:2]
    int dirFila[4] = {-1, 1, 0, 0};
    int dirCol[4]  = {0, 0, -1, 1};
    const char* nombreDir[4] = {"Arriba", "Abajo", "Izquierda", "Derecha"};

    int valores[4] = {0, 0, 0, 0};
    bool valida[4] = {false, false, false, false};

    // Lanzar dados y verificar direcciones validas (d <= PV actual y no salir). [file:2]
    for (int i = 0; i < numDados; i++) {
        valores[i] = dado.lanzar(pvInicial);

        int nuevaFila = fila + dirFila[i] * valores[i];
        int nuevaCol  = col  + dirCol[i] * valores[i];

        if (valores[i] <= j->getVidaActual() &&
            tablero->validarPosicion(nuevaFila, nuevaCol)) {
            valida[i] = true;
        }
    }

    // Contar direcciones validas. [file:2]
    int cuentaValidas = 0;
    int ultimaValida = -1;
    for (int i = 0; i < numDados; i++) {
        if (valida[i]) {
            cuentaValidas++;
            ultimaValida = i;
        }
    }

    if (cuentaValidas == 0) {
        cout << "No hay direcciones validas. El jugador pierde el turno.\n";
        jugadorActual = (jugadorActual + 1) % numJugadores;
        return;
    }

    int eleccion = ultimaValida;

    if (cuentaValidas > 1) {
        cout << "Direcciones validas:\n";
        for (int i = 0; i < numDados; i++) {
            if (valida[i]) {
                cout << i << ") " << nombreDir[i]
                     << " con " << valores[i] << " casillas\n";
            }
        }
        cout << "Elige direccion: ";
        cin >> eleccion;
        if (eleccion < 0 || eleccion >= numDados || !valida[eleccion]) {
            cout << "Opcion invalida, se anula el turno.\n";
            jugadorActual = (jugadorActual + 1) % numJugadores;
            return;
        }
    } else {
        cout << "Solo hay una direccion valida: "
             << nombreDir[ultimaValida]
             << " con " << valores[ultimaValida]
             << " casillas (movimiento automatico).\n";
    }

    moverJugador(jugadorActual,
                 dirFila[eleccion], dirCol[eleccion],
                 valores[eleccion]);

    mostrarEstado();

    // Pasar al siguiente jugador en orden fijo. [file:2]
    jugadorActual = (jugadorActual + 1) % numJugadores;
}
