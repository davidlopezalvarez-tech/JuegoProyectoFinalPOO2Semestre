//
// Created by aleji on 25/11/2025.
//

#ifndef PROYECTO_FINAL_CASILLA_H
#define PROYECTO_FINAL_CASILLA_H


class Casilla {
protected:
    int fila;
    int columna;
    int tipo;

public:
    Casilla(int fila, int columna, int tipo);

    int getFila();
    int getColumna();
    int getTipo();

    void setTipo(int nuevoTipo);

    virtual void efecto()=0;
};


#endif //PROYECTO_FINAL_CASILLA_H