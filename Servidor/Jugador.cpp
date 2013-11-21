#include "Jugador.h"

#define LEN_MSJ 5

#define MOVER '0'
#define BORRAR '1'
#define INSERTAR '2'
#define TERMINAR '3'

#define ENVIAR 0
#define RECIBIR 1

#define LARGO_VECTOR 4
#define IP "127.0.0.1"

using std::string;

Jugador::Jugador(string& nombre, int enviar, int recibir) :
    puerto_enviar(enviar), puerto_recibir(recibir), puntaje(0), jugada_actual(NULL){
    id = new string(nombre);
    sockets = new sockets_jugador_t();

    sockets->enviar = NULL;
    sockets->recibir = NULL;
    sockets->enviar_cli = NULL;
    sockets->recibir_cli = NULL;

    emisor = new EmisorResultados();
    mutex = new Mutex();
    emisor->agregar_mutex(mutex);
}

Jugador::~Jugador(){
    delete id;
    delete emisor;
    delete mutex;
    delete sockets;
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

void Jugador::enviarCelda(celda_t& celda){
    msj_celda_t mensaje;
    mensaje.tipo = INSERTAR;
    mensaje.celda = celda;
    sockets->enviar_cli->enviar(&mensaje, sizeof(mensaje));
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
    if(sockets->recibir_cli->recibir(&nuevo_par, sizeof(par_t)) == -1)
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
    enviarPuntaje();
    return true;
}

void Jugador::cerrarJugador(){
    emisor->join();
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
    emisor->finalizar();
}

int Jugador::prepararSocket(int tipo, int puerto){
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

int Jugador::prepararSocketEnviar(){
    if(sockets) return 5;
    return prepararSocket(ENVIAR, puerto_enviar);
}

int Jugador::prepararSocketRecibir(){
    if(sockets) return 5;
    return prepararSocket(RECIBIR, puerto_recibir);
}

void Jugador::enviarBorrados(){
    emisor->correr();
}

void Jugador::enviarPuntaje(){
    std::stringstream ss;
    ss << jugada_actual->verPuntos();
    string puntos(ss.str());

    msj_puntos_t msj;
    msj.tipo = TERMINAR;
    char vector[] = {msj.indice1, msj.indice2, msj.indice3, msj.indice4};
    int len = puntos.length(), contador = len-1;

    for(int i=LARGO_VECTOR-1; i<=0; i--){
        if(contador >= 0)
            vector[i] = '0';
        else
            vector[i] = puntos[contador];
        contador--;
    }

    sockets->enviar_cli->enviar(&msj, sizeof(msj));
}
