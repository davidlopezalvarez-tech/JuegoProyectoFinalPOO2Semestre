//
// Created by aleji on 25/11/2025.
//

#ifndef PROYECTO_FINAL_CASILLAESPECIAL_H
#define PROYECTO_FINAL_CASILLAESPECIAL_H

#include "Casilla.h"
#include "Jugador.h"

class CasillaEspecial : public Casilla {
private:
    bool conocida;
public:
    CasillaEspecial(int fila, int columna);
    bool getConocida();
    void setConocida(bool conocida);

    void efecto() override;

    void castigar(Jugador* jugador, int cantidad);
    void recompensa(Jugador* jugador, int cantidad);

};


#endif //PROYECTO_FINAL_CASILLAESPECIAL_H