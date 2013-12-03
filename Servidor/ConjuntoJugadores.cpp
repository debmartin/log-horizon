#include "ConjuntoJugadores.h"
#include <unistd.h>
#include <cstring>

#define MILISEGUNDOS 10
#define INVALIDA -1
#define PUNTAJE_PERDEDOR 0

#define SIN_GANADOR "No hay ganador"

using std::string;
using std::vector;

ConjuntoJugadores::ConjuntoJugadores(Mutex* mutex_recibir) :
    mejor_jugador(NULL), id_receptor(NULL), mutex(mutex_recibir){
    jugadores = new vector<Jugador*>();
}

ConjuntoJugadores::~ConjuntoJugadores(){
    for(size_t i = 0; i < jugadores->size(); i ++)
        delete (*jugadores)[i];
    delete jugadores;
}

bool ConjuntoJugadores::agregar(usuario_t& usuario){
    Jugador* nuevo_jugador = new Jugador(usuario);
//    if(jugadores->pertenece(nuevo_jugador->Id()))
//        return false;
    jugadores->push_back(nuevo_jugador);
    nuevo_jugador->agregarMutex(mutex);
    nuevo_jugador->agregarIdActual(id_receptor);
    nuevo_jugador->esperarJugada();
    nuevo_jugador->enviarBorrados();
    return true;
}

void ConjuntoJugadores::enviarPuntaje(int puntos){
    int len = jugadores->size();
    for(int i=0; i<len; i++){
        if(i != *id_receptor)
            (*jugadores)[i]->enviarPuntaje(PUNTAJE_PERDEDOR);
        else
            (*jugadores)[i]->enviarPuntaje(puntos);
    }
}

void ConjuntoJugadores::enviarTablero(Lista<celda_t*>& celdas){
    int len = jugadores->size();
    for(int i=0; i<len; i++){
        (*jugadores)[i]->enviarTablero(celdas);
    }
    for(int i=0; i<len; i++){
        (*jugadores)[i]->terminarEmisionTablero();
    }
}

Jugada* ConjuntoJugadores::obtenerJugada(){
    while(! id_receptor){
        usleep(MILISEGUNDOS * 1000);
    }
    return ((*jugadores)[*id_receptor]->obtenerJugada());
}

bool ConjuntoJugadores::sumarPuntos(){
    if (!id_receptor) return false;
    bool res = (*jugadores)[*id_receptor]->sumarPuntos();
    if (! mejor_jugador || (mejor_jugador->getPuntaje() < ((*jugadores)[*id_receptor]->getPuntaje())))
            mejor_jugador = (*jugadores)[*id_receptor];
    return res;
}

int ConjuntoJugadores::verPuntajeMax(){
    if (!mejor_jugador) return 0;
    return (mejor_jugador->getPuntaje());
}

void prepararMensaje(char tipo, string& original, string& destino){
    int largo_orig = original.length();
    uint32_t len = htonl((uint32_t) largo_orig);
    destino = tipo;
    destino += len;
    destino += original;
}

void ConjuntoJugadores::enviarGanador(bool hay_ganador){
    string ganador;
    ganador += hay_ganador ? mejor_jugador->Id() : SIN_GANADOR;

    string msj_char;
    prepararMensaje(TERMINAR, ganador, msj_char);

    int len = jugadores->size();
    for(int i=0; i<len; i++){
        (*jugadores)[i]->enviarMensaje(&msj_char, sizeof(msj_char));
    }
}

bool ConjuntoJugadores::terminarJugada(){
    if (!id_receptor) return false;
    return ((*jugadores)[*id_receptor]->terminarJugada());
}

void ConjuntoJugadores::encolarBorrados(Tablero* tablero){
    int len = jugadores->size();
    for(int i=0; i<len; i++){
        (*jugadores)[i]->encolarBorrados(tablero);
    }
}

void ConjuntoJugadores::cerrar(){
    int len = jugadores->size();
    for(int i=0; i<len; i++){
        (*jugadores)[i]->cerrar();
    }
}
