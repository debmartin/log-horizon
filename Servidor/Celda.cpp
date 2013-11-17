#include "Celda.h"
#include <iostream>
char rand_color(){
    switch (rand() % 5) {
        case 0:
            return ROJO;
        case 1:
            return AMARILLO;
        case 2:
            return VERDE;
        case 3:
            return AZUL;
        default:
            return VIOLETA;
    }
}

Celda::Celda() : color(rand_color()), tipo(BUTTON){
}

Celda::Celda(char cel_color, char cel_tipo) :
    color(cel_color), tipo(cel_tipo){
}

Celda::~Celda(){
}

void Celda::rellenar(){
    tipo = BUTTON;
    color = rand_color();
}

void Celda::rellenar(char cel_color, char cel_tipo){
    tipo = cel_tipo;
    color = cel_color;
}

bool Celda::esEstrella(){
    return (tipo == ESTRELLA);
}

bool Celda::esVacia(){
    return (tipo == VACIO);
}

Celda& Celda::operator=(const Celda& otra){
    if (this == &otra) return *this;

    color = otra.color;
    tipo = otra.tipo;
    return *this;
}
