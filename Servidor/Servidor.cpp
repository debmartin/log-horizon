#include "Servidor.h"

#include  <cstring>
#include "funciones_auxiliares.h"
#define PUERTO_MAX 65535

#define ARCH_USUARIOS "./usuarios.json"

#define OK '1'
#define ERROR '0'

#define CREAR_PARTIDA '0'
#define UNIRSE_PARTIDA '1'

#define ARCH_NIVELES "./niveles.json"

#define AVANZAR '0'
#define RETROCEDER '1'
#define ELEGIR '2'

using std::string;

Servidor::Servidor() :
    proximo_puerto(PUERTO_ESCUCHA+1), cliente_actual(NULL){
    socket_escucha = new Socket();
    socket_escucha->asignar_direccion(PUERTO_ESCUCHA);
    if (socket_escucha->reusar() == -1) throw ServidorCrearSocket();
    if (socket_escucha->asociar() == -1) throw ServidorCrearSocket();
    if (socket_escucha->escuchar() == -1) throw ServidorCrearSocket();

    nuevo_usuario = new usuario_t();

    niveles = new vector<nivel_t*>();
    partidas = new vector<nivel_t*>();
}

Servidor::~Servidor(){
    delete socket_escucha;
}

void Servidor::cargarNiveles(){

}

void Servidor::recibirConexion(sockets_jugador_t& sockets){
    if(cliente_actual) return;

    cliente_actual = new Socket();
    socket_escucha->aceptar(*cliente_actual);
}

int Servidor::asignarPuerto(Socket& sockfd){
    bool asociado = false;
    while(! asociado){
        sockfd.asignar_direccion(proximo_puerto);
        if (sockfd.reusar() == -1) return 1;
        asociado = (sockfd.asociar() != -1);

        proximo_puerto++;
        if (proximo_puerto == PUERTO_MAX)
            proximo_puerto = PUERTO_ESCUCHA +1;
    }
    if (sockfd.escuchar() == -1) return 2;
    return 0;
}

void Servidor::enviarPuertos(){
    nuevo_usuario->sockets->enviar = new Socket();
    nuevo_usuario->sockets->recibir = new Socket();
    Socket* socks[] = {nuevo_usuario->sockets->recibir, nuevo_usuario->sockets->enviar};

    for(int i = 0; i < 2; i++){
        Socket& actual = *(socks[i]);
        asignarPuerto(actual);
        uint32_t nro_puerto = actual.ver_puerto();
        cliente_actual->enviar(&nro_puerto, sizeof(uint32_t));
    }
    cliente_actual->cerrar_enviar_recibir();
    delete cliente_actual;
    cliente_actual = NULL;
}

bool Servidor::aceptarSubConexiones(){
    nuevo_usuario->sockets->enviar_cli = new Socket();
    nuevo_usuario->sockets->recibir_cli = new Socket();
    Socket* sockfds[] = {nuevo_usuario->sockets->enviar, nuevo_usuario->sockets->recibir};
    Socket* socks_cli[] = {nuevo_usuario->sockets->enviar_cli, nuevo_usuario->sockets->recibir_cli};

    for(int i=0; i < 2 ; i++){
        if (sockfds[i]->aceptar(*(socks_cli[i])) == -1) {
            delete nuevo_usuario->sockets->enviar_cli;
            delete nuevo_usuario->sockets->recibir_cli;
            return false;
        }
    }
    return true;
}

void Servidor::recibirUsuarioContrasenia(){
    nuevo_usuario->nombre = new string();
    recibirMsj(*(nuevo_usuario->sockets->recibir_cli), *(nuevo_usuario->nombre));

    nuevo_usuario.contrasenia = new string();
    recibirMsj(*(nuevo_usuario->sockets->recibir_cli), *(nuevo_usuario->contrasenia));
}

void Servidor::verificarUsuario(){
    ArchivoDirecto archivo(ARCH_USUARIOS);
    archivo.abrir();
    string aux_contrasenia;
    archivo.obtener(*(nuevo_usuario->nombre), aux_contrasenia);
    char res = (aux_contrasenia == *(nuevo_usuario->contrasenia)) ? OK : ERROR;
    nuevo_usuario->sockets->enviar_cli->enviar(&res, sizeof(char));
}

int Servidor::definirPartida(){
    char tipo_partida = ' ';
    nuevo_usuario->sockets->recibir_cli->recibir(&tipo_partida, sizeof(char));
    return (tipo_partida - '0');
}

int Servidor::crearPartida(){
    vector<nivel_t*>::iterator it = niveles->begin():
    for(; it != niveles->end(); ++it){
        enviarMsjPrefijo(*(nuevo_usuario->sockets->enviar_cli), it->nombre->c_str(), it->nombre->length());
        uint32_t puntos = it->puntaje;
        nuevo_usuario->sockets->enviar_cli->enviar(&puntos, TAM_UINT32);
        uint32_t cant_jug = it->cant_jugadores;
        nuevo_usuario->sockets->enviar_cli->enviar(&cant_jug, TAM_UINT32);
    }
}


//Socket* sock_actual;
//
//    for(int i=0; i<4; i++){
//        switch (i){
//            case 0:
//                sock_actual = sockets->enviar_cli;
//                break;
//            case 1:
//                sock_actual = sockets->enviar;
//                break;
//            case 2:
//                sock_actual = sockets->recibir_cli;
//                break;
//            case 3:
//                sock_actual = sockets->recibir;
//        }
//        sock_actual->cerrar_enviar_recibir();
//        delete sock_actual;
//    }
