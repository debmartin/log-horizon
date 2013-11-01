#include "Matriz.h"
#include <iostream>

#define TAM 10

#define ROJO 'r'
#define AMARILLO 'y'
#define VERDE 'g'
#define AZUL 'b'
#define VIOLETA 'v'
#define ORO 'o'
#define VACIO 'n'
#define BUTTON 'c'
#define MINIBARH 'h'
#define MINIBARV 'p'
#define ESTRELLA 's'
char colores[] = {ROJO, AMARILLO, VERDE, AZUL, VIOLETA, ORO, VACIO};
char tipos[] = {BUTTON, MINIBARH, MINIBARV, ESTRELLA, VACIO};

char Celda::rand(41);

char random(Celda* This){
    This->rand = 403* This->rand + (This->rand^2) / 17 + 31;
    return This->rand;
}

Celda::Celda() :
    color(colores[random(this)%5]), tipo(random(this)%3) {
}

Celda::Celda(char cel_color, char cel_tipo) :
    color(cel_color), tipo(cel_tipo){
}

Celda::~Celda(){
}

bool Celda::esEstrella(){
    return (tipo == ESTRELLA);
}

Celda& Celda::operator=(const Celda& otra){
    if (this == &otra) return *this;

    color = otra.color;
    tipo = otra.tipo;
    return *this;
}

Matriz::Matriz() : tamanio(TAM){
    tabla = new Celda*[TAM];
    for(int i=0; i<TAM; i++){
        tabla[i] = new Celda[TAM];
    }
}

Matriz::~Matriz(){
    for(int i=0; i<TAM; i++){
        delete tabla[i];
    }
    delete tabla;
}

int Matriz::intercambiar(Dimension& una, Dimension& otra){
    int unaf = una.getFila(), unac = una.getCol();
    int otraf = otra.getFila(), otrac = otra.getCol();

    if ((! una.esValida()) || (! otra.esValida()))
        return 1;
    if (unaf >= tamanio || unac >= tamanio || otrac >= tamanio || otraf >= tamanio)
        return 1;

    Celda aux = tabla[unaf][unac];
    tabla[unaf][unac] = tabla[otraf][otrac];
    tabla[otraf][otrac] = aux;

    return 0;
}

Celda Matriz::getCelda(int i, int j){
    if ((i >= tamanio) || (j >= tamanio)) throw "Fuera de rango";

    return tabla[i][j];
}

Celda* Matriz::operator[](int i){
    if (i >= tamanio) throw "Fuera de rango";

    return tabla[i];
}

Celda& operator[](Dimension& pos){
    int fila = pos.getFila(), col = pos.getCol();
    if ((col >= tamanio) || (fila >= tamanio))
        throw "Fuera de rango";

    return tabla[fila][col];
}

void Matriz::imprimir(){
    std::cout << "      ";
    for(int i=0; i<tamanio; i++)
        std::cout << i << "     ";
    std::cout << '\n' << '\n';

    for(int i=0; i<tamanio; i++){
        std::cout << i << "     ";
        for(int j=0; j<tamanio; j++){
            Celda salida = tabla[i][j];
            std::cout << '(' << salida.getColor() << ',' << salida.getTipo() << ')';
//            if (salida >= 10)
//                std::cout << "    ";
//            else
                std::cout << "     ";
        }
        std::cout << '\n';
    }
    std::cout << '\n' << '\n';
}

