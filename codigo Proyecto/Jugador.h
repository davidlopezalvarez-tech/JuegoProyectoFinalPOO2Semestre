//
// Created by aleji on 20/11/2025.
//
#ifndef FINAL_JUGADOR_H
#define FINAL_JUGADOR_H

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
    Jugador(int id, int maxVidaInicial,int filaInicial, int colInicial);

    int getId();
    int getMaxVida();
    int getVidaActual();
    bool estaEliminado();

    int getFila();
    int getColumna();

    void setVida(int vidaActual);
    void setPosicion(int fila, int columna);
};

#endif //FINAL_JUGADOR_H