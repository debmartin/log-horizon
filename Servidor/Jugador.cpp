#include "Jugador.h"
#include "Constantes.h"

#define LEN_MSJ 5

#define ENVIAR 0
#define RECIBIR 1

#define IP "127.0.0.1"

using std::string;

Jugador::Jugador(usuario_t& usuario) :
    puntaje(0), jugada_actual(NULL), mutex_receptor(NULL){
    id = usuario.nombre;
    sockets = usuario.sockets;

    emisor_tab = new EmisorTablero();
    emisor = new EmisorResultados();
    mutex_emisor = new Mutex();

    emisor->agregar_mutex(mutex_emisor);
    emisor->agregar_socket(usuario.sockets->enviar_cli);
    emisor_tab->agregarMutex(mutex_emisor);
    emisor_tab->agregarSocket(usuario.sockets->enviar_cli);

    receptor = new ReceptorJugada();
    receptor->agregarSocket(usuario.sockets->recibir_cli);
}

Jugador::~Jugador(){
    if(jugada_actual) delete jugada_actual;
    delete receptor;
    delete emisor;
    delete mutex_emisor;
}

Jugada* Jugador::obtenerJugada(){
    recibirPar();
    return jugada_actual;
}

void Jugador::agregarIdActual(int* id_jugador){
    receptor->agregarIdActual(id_jugador);
}

bool Jugador::sumarPuntos(){
    if (! jugada_actual)
     return false;
    puntaje += jugada_actual->verPuntos();
    return true;
}

void Jugador::agregarMutex(Mutex* mutex_recibir){
    receptor->agregarMutex(mutex_recibir);
}

void Jugador::enviarTablero(Lista<celda_t*>& celdas){
    emisor_tab->agregarTablero(&celdas);
    emisor_tab->correr();
}

void Jugador::terminarEmisionTablero(){
    emisor_tab->join();
    delete emisor_tab;
    emisor_tab = NULL;
}

bool Jugador::terminarJugada(){
    if(! jugada_actual->sinBorrados())
        return false;
    delete jugada_actual;
    jugada_actual = NULL;
    receptor->terminarActual();
    return true;
}

bool Jugador::recibirPar(){
    par_t nuevo_par;
    if(! receptor->obtenerPar(nuevo_par))
        return false;

    Dimension pos1(nuevo_par.pos1.fila, nuevo_par.pos1.columna);
    Dimension pos2(nuevo_par.pos2.fila, nuevo_par.pos2.columna);

    jugada_actual = new Jugada(pos1, pos2);
    return true;
}

bool Jugador::encolarBorrados(Tablero* tablero){
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

void Jugador::cerrar(){
    receptor->terminar();
    emisor->finalizar();
    receptor->join();
    emisor->join();
    emisor->finalizar();
}

void Jugador::enviarBorrados(){
    emisor->correr();
}

void Jugador::esperarJugada(){
    receptor->correr();
}

void Jugador::enviarPuntaje(int puntos){
    std::stringstream ss;
    ss << puntos;
    string str_puntos(ss.str());

    char msj[LEN_MSJ];
    msj[0] = PUNTOS;
    int len = str_puntos.length(), contador = len-1;

    for(int i = LEN_MSJ-2; i>=0; i--){
        char& indice = msj[i];
        if(contador >= 0)
            indice = str_puntos[contador];
        else
            indice = '0';
        contador--;
    }
    enviarMensaje(msj, LEN_MSJ);
}

void Jugador::enviarMensaje(void* mensaje, int len){
    mutex_emisor->bloquear();
    sockets->enviar_cli->enviar(mensaje, len);
    mutex_emisor->desbloquear();
}
