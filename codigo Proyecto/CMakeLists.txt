cmake_minimum_required(VERSION 4.0)
project(codigo_Proyecto)

set(CMAKE_CXX_STANDARD 20)

add_executable(codigo_Proyecto main.cpp
        Casilla.cpp
        Casilla.h
        CasillaEspecial.cpp
        CasillaEspecial.h
        CasillaNormal.cpp
        CasillaNormal.h
        Jugador.cpp
        Jugador.h
        Juego.cpp
        Juego.h
        Dado.cpp
        Dado.h
        Tablero.cpp
        Tablero.h)
