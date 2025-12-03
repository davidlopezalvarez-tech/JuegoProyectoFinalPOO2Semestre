#include <iostream>
#include "Juego.h"
using namespace std;

int main() {
    int opcionMenu = 0;
cout <<"hola";
    cout << "1) Nuevo juego\n";
    cout << "2) Salir\n";
    cout << "Elige opcion: ";
    cin >> opcionMenu;

    if (opcionMenu != 1) {
        return 0;
    }

    int dificultad;
    cout << "Selecciona dificultad (1=Facil, 2=Medio, 3=Dificil): ";
    cin >> dificultad;

    Juego juego(dificultad);
    juego.iniciarPartida();

    int opcionTurno = 0;
    do {
        cout << "\n--- Menu de turno ---\n";
        cout << "1) Lanzar dados y mover\n";
        cout << "2) Salir de la partida\n";
        cout << "Elige opcion: ";
        cin >> opcionTurno;

        if (opcionTurno == 1) {
            juego.turnoJugador();
        }
    } while (opcionTurno != 2);

    return 0;
}
