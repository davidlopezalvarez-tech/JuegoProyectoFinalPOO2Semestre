cmake_minimum_required(VERSION 4.0)
project(PROYECTO_FINAL)

set(CMAKE_CXX_STANDARD 20)

add_executable(PROYECTO_FINAL main.cpp
        Final/Jugador.h
        Final/Casilla.h
        Final/CasillaNormal.cpp
        Final/CasillaNormal.h
        Final/CasillaEspecial.cpp
        Final/CasillaEspecial.h
        Final/Casilla.cpp)
