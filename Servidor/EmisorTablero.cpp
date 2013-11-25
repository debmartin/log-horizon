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
        msj_celda_t msj;
        msj.celda.col = iter.verActual()->col;
        msj.celda.fila = iter.verActual()->fila;
        msj.celda.tipo = iter.verActual()->tipo;
        msj.celda.color = iter.verActual()->color;
        msj.tipo = INSERTAR;
        socket->enviar(&msj, sizeof(msj_celda_t));
        iter.avanzar();
    }
}
