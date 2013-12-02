#include "ReceptorJugada.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Constantes.h"

#define MILISEGUNDOS 10


int ReceptorJugada::cant_receptores = 0;

ReceptorJugada::ReceptorJugada() :
    par_recibido(NULL), socket(NULL), seguir(true){
    *id_jugador = cant_receptores++;
    mutex_par_recibido = new Mutex();
}

ReceptorJugada::~ReceptorJugada(){
    delete mutex_par_recibido;
}

void ReceptorJugada::agregarSocket(Socket *nuevo_socket){
    socket = nuevo_socket;
}

void ReceptorJugada::agregarMutex(Mutex *nuevo_mutex){
    mutex = nuevo_mutex;
}

void ReceptorJugada::agregarIdActual(int* jugador_actual){
    id_jugador = jugador_actual;
}

void convertir_a_par(char recibidos[LEN_MSJ], par_t& par_rec){
    par_rec.pos1.columna = recibidos[0];
    par_rec.pos1.fila = recibidos[1];
    par_rec.pos2.columna = recibidos[2];
    par_rec.pos2.fila = recibidos[3];
}

void ReceptorJugada::funcion_a_correr(){
    if (!socket || !mutex) return;
    mutex_par_recibido->bloquear();
    while (seguir){
        if(par_recibido){
            mutex_par_recibido->desbloquear();
            usleep(MILISEGUNDOS * 1000);
            mutex_par_recibido->bloquear();
            continue;
        }
        par_recibido = new par_t();
        char recibidos[LEN_MSJ];
        int res = socket->recibir(recibidos, sizeof(char) * LEN_MSJ);
        convertir_a_par(recibidos, *par_recibido);
        if(res == -1) continue;
        if(res == 0) {
            if (par_recibido) delete par_recibido;
            par_recibido = NULL;
            return;
        }

        if(id_jugador){
            socket->enviar(par_recibido, sizeof(par_t));
            delete par_recibido;
            par_recibido = NULL;
        } else {
            mutex->bloquear();
            id_jugador = new int(id_propio);
            mutex->desbloquear();
        }
    }
    mutex_par_recibido->desbloquear();
}

void ReceptorJugada::terminar(){
    seguir = false;
}

bool ReceptorJugada::obtenerPar(par_t& nuevo_par){
    mutex_par_recibido->bloquear();
    if(! par_recibido){
        mutex_par_recibido->desbloquear();
        return false;
    }

    nuevo_par.pos1.columna = par_recibido->pos1.columna;
    nuevo_par.pos1.fila = par_recibido->pos1.fila;
    nuevo_par.pos2.columna = par_recibido->pos2.columna;
    nuevo_par.pos2.fila = par_recibido->pos2.fila;
    mutex_par_recibido->desbloquear();
    return true;
}

void ReceptorJugada::terminarActual(){
    mutex_par_recibido->bloquear();
    delete par_recibido;
    par_recibido = NULL;

    mutex->bloquear();
    delete id_jugador;
    id_jugador = NULL;
    mutex->desbloquear();

    mutex_par_recibido->desbloquear();
}
