#include "Nivel.h"
#define PUNT_INICIAL 0

#define PUERTO_ENVIAR 8000
#define PUERTO_RECIBIR 8001


using std::string;

Nivel::Nivel(Dimension& tam, char** estructura, int max_puntos) :
    puntaje_objetivo(max_puntos), puntaje_max(PUNT_INICIAL), datos_jug(NULL) {
    tablero = new Tablero(tam, estructura);


}

Nivel::~Nivel(){
    delete tablero;
    if(datos_jug) delete datos_jug;
}

void Nivel::agregarJugador(string& nombre){
    datos_jug = new DatosJugador(PUERTO_ENVIAR, PUERTO_RECIBIR);
}

void Nivel::imprimirTablero(){
    tablero->imprimir();
}



//void Nivel::jugar(){
//    while(tablero->hayMovimientos() && (puntaje_max < puntaje_objetivo)){
//        Jugada* nueva_jugada = jugador->obtenerJugada();
//        tablero->intercambiar(nueva_jugada);
//        tablero->estabilizar();
//        jugador->sumarPuntos();
//        int puntos_jug = jugador->getPuntaje();
//        if (puntaje_max < puntos_jug)
//            puntaje_max = puntos_jug;
//        jugador->enviarBorrados(tablero);
//        jugador->terminarJugada();
//    }
//
//}
