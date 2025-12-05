#include "Casilla.h"

Casilla::Casilla(int fila, int columna, int tipo) {
    this->fila = fila;
    this->columna = columna;
    this->tipo = tipo;
}
int Casilla::getFila() {
    return fila;
}
int Casilla::getColumna() {
    return columna;
}
int Casilla::getTipo() {
    return tipo;
}
void Casilla::setTipo(int nuevoTipo) {
    tipo=nuevoTipo;
}
