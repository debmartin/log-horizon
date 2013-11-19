#include "Jugada.h"

Jugada::Jugada(Dimension& principio, Dimension& fin) : puntos(0){
    pos1 = new Dimension(principio);
    pos2 = new Dimension(fin);
    borrados = new Cola<Dimension*>(true);
}

Jugada::~Jugada(){
    delete pos1;
    delete pos2;
}

bool Jugada::sumarPuntos(int cant_borrados, bool con_estrella, bool por_caida){
    int amplificador = por_caida ? 2 : 1;
    if (con_estrella) {
        puntos += PUNTAJE_ESTRELLA * cant_borrados * amplificador;
    } else if (cant_borrados < 3) {
        return false;
    } else {
        puntos += PUNTAJE_LINEA * (cant_borrados -2) * cant_borrados * amplificador;
    }
    return true;
}

int Jugada::verPuntos(){
    return puntos;
}

Dimension Jugada::posicion1(){
    return *pos1;
}

Dimension Jugada::posicion2(){
    return *pos2;
}

void Jugada::agregarBorrado(Dimension* pos){
    *borrados += pos;
}

Dimension* Jugada::obtenerProxBorrado(){
    return borrados->desencolar();
}
