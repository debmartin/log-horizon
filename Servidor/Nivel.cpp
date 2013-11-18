#include "Nivel.h"

using std::string;

Nivel::Nivel() :
    puntaje_objetivo(300), puntaje_max(0){
    modificados = new Lista<Dimension*>(true);
    tablero = new Tablero(7, modificados);
    jugadores = new Hash<Jugador*>(true);
}

Nivel::~Nivel(){
    delete tablero;
    delete jugadores;
    delete modificados;
}

void Nivel::agregarJugador(string& nombre){
    Jugador* nuevo = new Jugador(nombre);
    jugadores->guardar(nombre, nuevo);
}

void Nivel::imprimirTablero(){
    tablero->imprimir();
}

void Nivel::jugar(){
//    while(tablero->hayMovimientos()){
//        Jugada* nueva_jugada = set_jugadores->obtenerJugada();
//        tablero->intercambiar(nueva_jugada);
//        tablero->estabilizar();
//    }
}
