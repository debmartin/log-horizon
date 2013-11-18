#include "Jugador.h"

using std::string;

Jugador::Jugador(string& nombre) : puntaje(0), jugada_actual(NULL){
    id = new string(nombre);
}

Jugador::~Jugador(){
    delete id;
}

//Jugada* Jugador::obtenerJugada(){
//    Jugada* nueva_jugada = new Jugada(pos1, pos2);
//    jugada_actual = nueva_jugada;
//    return nueva_jugada;
//}

bool Jugador::sumarPuntos(){
    if (! jugada_actual)
     return false;
    puntaje += jugada_actual->verPuntos();
    return true;
}
