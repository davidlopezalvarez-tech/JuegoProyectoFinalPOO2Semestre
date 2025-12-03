//
// Created by aleji on 25/11/2025.
//

#include "CasillaEspecial.h"
#include "Jugador.h"
CasillaEspecial::CasillaEspecial(int fila, int columna) : Casilla(fila, columna, -2) {
    conocida=false;
}

bool CasillaEspecial::getConocida() {
    return conocida;
}
void CasillaEspecial::setConocida(bool conocida) {
    this->conocida = conocida;
}
void CasillaEspecial::efecto() {
    // El efecto se hace en Juego
}
void CasillaEspecial::castigar(Jugador* jugador, int cantidad) {
    if (jugador != nullptr) {
        int vida = jugador->getVidaActual();
        jugador->setVida(vida - cantidad);
    }
}

void CasillaEspecial::recompensa(Jugador* jugador, int cantidad) {
    if (jugador != nullptr) {
        int vida = jugador->getVidaActual();
        jugador->setVida(vida + cantidad);
    }
}