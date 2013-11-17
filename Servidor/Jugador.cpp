#include "Jugador.h"



using std::string;

Jugador::Jugador(string& nombre) : puntaje(0){
    id = new string(nombre);
}

Jugador::~Jugador(){
    delete id;
}

