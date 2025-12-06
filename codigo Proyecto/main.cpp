#include <iostream>
#include "Juego.h"
#include "GestorPartida.h"

using namespace std;

int main() {
    GestorPartida gestor;
    Juego* juego = nullptr;
    int opcionMenuPrincipal = 0;

    do {
        cout << "\n=== MENU PRINCIPAL ===\n";
        cout << "1) Nuevo juego\n";
        cout << "2) Cargar partida\n";
        cout << "3) Eliminar partida\n";
        cout << "4) Salir\n";
        cout << "Elige opcion: ";
        cin >> opcionMenuPrincipal;

        if (opcionMenuPrincipal == 1) {
            // Nuevo juego
            int dificultad;
            int cantidadJugadores;

            cout << "Selecciona dificultad (1=Facil, 2=Medio, 3=Dificil): ";
            cin >> dificultad;

            cout << "Cantidad de jugadores (1 a 4): ";
            cin >> cantidadJugadores;

            // destruir juego anterior si existia
            delete juego;
            juego = new Juego(dificultad, cantidadJugadores);
            juego->iniciarPartida();

            // Menu dentro de la partida
            int opcionPartida = 0;
            do {
                cout << "\n--- Menu de partida ---\n";
                cout << "1) Lanzar dados y mover\n";
                cout << "2) Guardar partida\n";
                cout << "3) Rendirse y volver al menu principal\n";
                cout << "Elige opcion: ";
                cin >> opcionPartida;

                if (opcionPartida == 1) {
                    juego->turnoJugador();
                } else if (opcionPartida == 2) {
                    int numeroPartida;
                    gestor.mostrarPartidas();
                    cout << "Elige numero de partida para guardar (1-3): ";
                    cin >> numeroPartida;
                    if (gestor.guardarPartida(numeroPartida, juego)) {
                        cout << "Partida guardada correctamente.\n";
                    } else {
                        cout << "No se pudo guardar la partida.\n";
                    }
                } else if (opcionPartida == 3) {
                    int guardar;
                    cout << "¿Deseas guardar o actualizar la partida antes de salir?\n";
                    cout << "1) Si\n";
                    cout << "2) No\n";
                    cout << "Elige opcion: ";
                    cin >> guardar;

                    if (guardar == 1) {
                        int numeroPartida;
                        gestor.mostrarPartidas();
                        cout << "Elige numero de partida para guardar (1-3): ";
                        cin >> numeroPartida;
                        if (gestor.guardarPartida(numeroPartida, juego)) {
                            cout << "Partida guardada correctamente.\n";
                        } else {
                            cout << "No se pudo guardar la partida.\n";
                        }
                    }

                    cout << "Has abandonado la partida.\n";
                }
            } while (opcionPartida != 3);

        } else if (opcionMenuPrincipal == 2) {
            // Cargar partida
            int numeroPartida;
            gestor.mostrarPartidas();
            cout << "Elige numero de partida para cargar (1-3): ";
            cin >> numeroPartida;

            Juego* juegoCargado = nullptr;
            if (gestor.cargarPartida(numeroPartida, juegoCargado)) {
                cout << "Partida cargada correctamente.\n";
                delete juego;
                juego = juegoCargado;

                // Mostrar estado inicial de la partida cargada
                juego->getTablero()->dibujarTablero();
                juego->mostrarEstado();

                // Menu dentro de la partida cargada
                int opcionPartida = 0;
                do {
                    cout << "\n--- Menu de partida ---\n";
                    cout << "1) Lanzar dados y mover\n";
                    cout << "2) Guardar partida\n";
                    cout << "3) Rendirse y volver al menu principal\n";
                    cout << "Elige opcion: ";
                    cin >> opcionPartida;

                    if (opcionPartida == 1) {
                        juego->turnoJugador();
                    } else if (opcionPartida == 2) {
                        int numeroGuardar;
                        gestor.mostrarPartidas();
                        cout << "Elige numero de partida para guardar (1-3): ";
                        cin >> numeroGuardar;
                        if (gestor.guardarPartida(numeroGuardar, juego)) {
                            cout << "Partida guardada correctamente.\n";
                        } else {
                            cout << "No se pudo guardar la partida.\n";
                        }
                    } else if (opcionPartida == 3) {
                        int guardar;
                        cout << "¿Deseas guardar o actualizar la partida antes de salir?\n";
                        cout << "1) Si\n";
                        cout << "2) No\n";
                        cout << "Elige opcion: ";
                        cin >> guardar;

                        if (guardar == 1) {
                            int numeroGuardar;
                            gestor.mostrarPartidas();
                            cout << "Elige numero de partida para guardar (1-3): ";
                            cin >> numeroGuardar;
                            if (gestor.guardarPartida(numeroGuardar, juego)) {
                                cout << "Partida guardada correctamente.\n";
                            } else {
                                cout << "No se pudo guardar la partida.\n";
                            }
                        }

                        cout << "Has abandonado la partida.\n";
                    }
                } while (opcionPartida != 3);
            } else {
                cout << "No se pudo cargar la partida seleccionada.\n";
            }

        } else if (opcionMenuPrincipal == 3) {
            // Eliminar partida
            int numeroPartida;
            gestor.mostrarPartidas();
            cout << "Elige numero de partida para eliminar (1-3): ";
            cin >> numeroPartida;

            if (gestor.eliminarPartida(numeroPartida)) {
                cout << "Partida eliminada correctamente.\n";
            } else {
                cout << "No se pudo eliminar la partida.\n";
            }
        }

    } while (opcionMenuPrincipal != 4);

    delete juego;
    return 0;
}
