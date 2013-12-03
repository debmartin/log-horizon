#include "EmisorTablero.h"

#include "Constantes.h"

EmisorTablero::EmisorTablero() :
    socket(NULL), mutex(NULL), celdas_tablero(NULL){
}

EmisorTablero::~EmisorTablero(){
}

void EmisorTablero::agregarSocket(Socket *nuevo_socket){
    socket = nuevo_socket;
}

void EmisorTablero::agregarMutex(Mutex *nuevo_mutex){
    mutex = nuevo_mutex;
}

void EmisorTablero::agregarTablero(Lista<celda_t*>* celdas){
    celdas_tablero = celdas;
}

void EmisorTablero::funcion_a_correr(){
    ListaIter<celda_t*> iter(celdas_tablero);
    while(! iter.alFinal()){
        celda_t* celda = iter.verActual();
        char msj[LEN_MSJ] = {INSERTAR, celda->col, celda->fila, celda->tipo, celda->color};
        socket->enviar(&msj, sizeof(char)* LEN_MSJ);
        iter.avanzar();
    }
}
