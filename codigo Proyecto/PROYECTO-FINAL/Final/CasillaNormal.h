//
// Created by aleji on 25/11/2025.
//

#ifndef PROYECTO_FINAL_CASILLANORMAL_H
#define PROYECTO_FINAL_CASILLANORMAL_H

#include "Casilla.h"

class CasillaNormal : public Casilla {

public:
    CasillaNormal(int fila, int columna);

    void efecto() override;
};


#endif //PROYECTO_FINAL_CASILLANORMAL_H