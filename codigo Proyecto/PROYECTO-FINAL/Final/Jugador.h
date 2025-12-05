//
// Created by aleji on 20/11/2025.
//

#ifndef FINAL_JUGADOR_H
#define FINAL_JUGADOR_H
#include "CasillaEspecial.h"
#include <string>
using namespace std;

class Jugador {

private:
    int id;
    int maxVida;
    int vidaActual;
    bool eliminado;
    int filaJugador;
    int columnaJugador;

public:
    Jugador();
    Jugador(int id, int maxVidaInicial, int posicionTablero, int filaInicial, int colInicial);


    int getId();
    int getMaxVida();
    int getVidaActual();
    bool estaEliminado();
    int getFila();

    void setVida(int vidaActual);





};


#endif //FINAL_JUGADOR_H