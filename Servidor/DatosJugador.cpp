#include "Jugador.h"

#define ENVIAR 0
#define RECIBIR 1

#define IP "127.0.0.1"


using std::string;

DatosJugador::DatosJugador(int enviar, int recibir) :
    puerto_enviar(enviar), puerto_recibir(recibir), jugador(NULL), thread(NULL){
    sockets = new sockets_jugador_t();

    sockets->enviar = NULL;
    sockets->recibir = NULL;
    sockets->enviar_cli = NULL;
    sockets->recibir_cli = NULL;
}

DatosJugador::~DatosJugador(){
    if(jugador) delete jugador;
    delete sockets;
    if (thread) delete thread;
}

void DatosJugador::crearJugador(string& nombre){
    jugador = new Jugador(nombre, sockets->enviar_cli, sockets->recibir_cli);
    thread = new ThreadJugador(jugador->emisor);
}

int DatosJugador::prepararSocket(int tipo, int puerto){
    Socket* sockfd = (tipo == ENVIAR) ? sockets->enviar : sockets->recibir;
    Socket* sockcli = (tipo == ENVIAR) ? sockets->enviar_cli : sockets->recibir_cli;

    sockfd = new Socket();
    sockfd->asignar_direccion(puerto, IP);
    if (sockfd->reusar() == -1) return 1;
    if (sockfd->asociar() == -1) return 2;
    if (sockfd->escuchar() == -1) return 3;

    sockcli = new Socket();
    if (sockfd->aceptar(*sockcli) == -1) return 4;
    return 0;
}

int DatosJugador::prepararSocketEnviar(){
    if(sockets) return 5;
    return prepararSocket(ENVIAR, puerto_enviar);
}

int DatosJugador::prepararSocketRecibir(){
    if(sockets) return 5;
    return prepararSocket(RECIBIR, puerto_recibir);
}

int DatosJugador::getPuntaje(){
    return jugador->getPuntaje();
}

bool DatosJugador::sumarPuntos(){
    return jugador->sumarPuntos();
}

Jugada* DatosJugador::obtenerJugada(){
    return jugador->obtenerJugada();
}

bool DatosJugador::terminarJugada(){
    return jugador->terminarJugada();
}

bool DatosJugador::recibirPar(){
    return jugador->recibirPar();
}

bool DatosJugador::encolarBorrados(Tablero* tablero){
    return jugador->enviarBorrados(tablero);
}

void DatosJugador::enviarBorrados(){
    thread->correr();
}

void DatosJugador::cerrarJugador(){
    thread->join();
    Socket* sock_actual;

    for(int i=0; i<4; i++){
        switch (i){
            case 0:
                sock_actual = sockets->enviar;
                break;
            case 1:
                sock_actual = sockets->enviar_cli;
                break;
            case 2:
                sock_actual = sockets->recibir;
                break;
            case 3:
                sock_actual = sockets->recibir_cli;
        }
        sock_actual->cerrar_enviar_recibir();
        delete sock_actual;
    }
}
