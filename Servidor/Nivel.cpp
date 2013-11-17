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

void Nivel::recibirJugada(Dimension& una, Dimension& otra, string& id_jugador){
    tablero->intercambiar(una, otra);
//    bool con_estrella = (*tablero)[una].esEstrella() || (*tablero)[otra].esEstrella();
//    if (cant > 0)
//        jugadores->obtener(id_jugador).verPrimero()->contarPuntos(cant, con_estrella);
}


//
//void Nivel::jugar(){
//    while(tablero->hayMovimientos()){
//        recibirJugada();
//    }
//}
