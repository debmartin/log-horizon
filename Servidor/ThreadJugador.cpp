#include "ThreadJugador.h"

ThreadJugador::ThreadJugador(EmisorResultados* emisor) :
    Thread(), emisor_res(emisor) {
}

ThreadJugador::~ThreadJugador(){
}

void ThreadJugador::funcion_a_correr(){
    emisor_res->funcion_a_correr();
}
