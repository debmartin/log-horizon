#include "Nivel.h"
#define PUNT_INICIAL 0

using std::string;

Nivel::Nivel(Dimension& tam, char** estructura, int max_puntos) :
    puntaje_objetivo(max_puntos), puntaje_max(PUNT_INICIAL), jugador(NULL){
    tablero = new Tablero(tam, estructura);
//    jugadores = new Hash<Jugador*>(true);
}

Nivel::~Nivel(){
    delete tablero;
//    delete jugadores;
    delete modificados;
}

void Nivel::agregarJugador(string& nombre){
    jugador = new Jugador(nombre);
//    jugadores->guardar(nombre, nuevo);
}

void Nivel::imprimirTablero(){
    tablero->imprimir();
}

void Nivel::jugar(){
    while(tablero->hayMovimientos() && (puntaje_max < puntaje_objetivo)){
        Jugada* nueva_jugada = jugador->obtenerJugada();
        tablero->intercambiar(nueva_jugada);
        tablero->estabilizar();
        jugador->sumarPuntos();
        int puntos_jug = jugador->getPuntaje();
        if (puntaje_max < puntos_jug)
            puntaje_max = puntos_jug;
        jugador->terminarJugada();
    }

}
