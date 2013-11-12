#include "Jugador.h"


#define PUNTAJE_LINEA 10
#define PUNTAJE_ESTRELLA 40

using std::string;

Jugador::Jugador(string& nombre) : puntaje(0){
    id = new string(nombre);
}

Jugador::~Jugador(){
    delete id;
}

bool Jugador::contarPuntos(int largo_linea, bool conEstrella){
    if (conEstrella) {
        puntaje += PUNTAJE_ESTRELLA * largo_linea;
    } else if (largo_linea < 3) {
        return false;
    } else {
        puntaje += PUNTAJE_LINEA * (largo_linea -2) * largo_linea;
    }
    return true;
}

