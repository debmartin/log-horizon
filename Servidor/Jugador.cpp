#include "Jugador.h"

#define LEN_MSJ 5

#define BORRAR '1'
#define MOVER '0'

using std::string;

Jugador::Jugador(string& nombre, Socket* enviar, Socket* recibir) :
    puntaje(0), jugada_actual(NULL), socket_recepcion(recibir){
    id = new string(nombre);
    emisor = new EmisorResultados();
    mutex = new Mutex();
    emisor->agregar_socket(enviar);
    emisor->agregar_mutex(mutex);
}

Jugador::~Jugador(){
    delete id;
    delete emisor;
    delete mutex;
}

Jugada* Jugador::obtenerJugada(){
    recibirPar();
    return jugada_actual;
}

bool Jugador::sumarPuntos(){
    if (! jugada_actual)
     return false;
    puntaje += jugada_actual->verPuntos();
    return true;
}

bool Jugador::terminarJugada(){
    if(! jugada_actual->sinBorrados())
        return false;
    delete jugada_actual;
    jugada_actual = NULL;
    return true;
}

bool Jugador::recibirPar(){
    par_t nuevo_par;
    if(socket_recepcion->recibir(&nuevo_par, sizeof(par_t)) == -1)
        return false;

    Dimension pos1(nuevo_par.pos1.fila, nuevo_par.pos1.columna);
    Dimension pos2(nuevo_par.pos2.fila, nuevo_par.pos2.columna);

    jugada_actual = new Jugada(pos1, pos2);
    return true;
}

bool Jugador::enviarBorrados(Tablero* tablero){
    if (jugada_actual->sinBorrados()) return false;

    while(! jugada_actual->sinBorrados()){
        Dimension* pos = jugada_actual->obtenerProxBorrado();
        dato_t dato1, dato2;
        dato1.valor1 = pos->y();
        dato1.valor2 = pos->x();
        dato2.valor1 = (*tablero)[*pos].getTipo();
        dato2.valor2 = (*tablero)[*pos].getColor();

        emisor->encolar_dato(BORRAR, dato1, dato2);
    }
    return true;
}
