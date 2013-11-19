#include "ThreadJugador.h"

#define MILISEGUNDOS 6

ThreadJugador::ThreadJugador(Jugador* nuevo) :
    Thread(), jugador(nuevo) {
    procesando_mutex = new Mutex();
}

ThreadJugador::~ThreadJugador(){
}

void ThreadJugador::funcion_a_correr(){
//    while(true){
//
//        usleep(MILISEGUNDOS * 1000);
//    }
}
