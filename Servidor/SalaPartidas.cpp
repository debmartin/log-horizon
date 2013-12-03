#include "SalaPartidas.h"
#include "../libs/json/include/json/json.h"

#define TAM_UINT32 sizeof(uint32_t)
#define OK '0'

using std::vector;
using std::map;

SalaPartidas::SalaPartidas(usuario_t* usuario, vector<nivel_t*>* v_niveles, map<int,partida_t*>* v_partidas) :
    nuevo_usuario(usuario), niveles(v_niveles), partidas(v_partidas), mutex_partidas(NULL){
}

SalaPartidas::~SalaPartidas(){
}

void SalaPartidas::agregarMutex(Mutex* mutex_v_partidas){
    mutex_partidas = mutex_v_partidas;
}

int SalaPartidas::definirTipoPartida(){
    char tipo_partida = ' ';
    int res = -1;
    while (res == -1){
        std::cout << *(nuevo_usuario->nombre);
        res = nuevo_usuario->sockets->recibir_cli->recibir(&tipo_partida, sizeof(char));
        if (res == 0) return CONEXION_ABORTADA;
    }
    return (tipo_partida - '0');
}

void enviarNivel(Socket& sockfd, nivel_t& nivel, string* creador = NULL, int jugadores = 0){
    Json::Value mensaje;
    Json::StyledWriter escritor;
    int info = (creador == NULL) ? INFO_CREAR : INFO_UNIRSE;
    mensaje["tipo"] = htonl((uint32_t) info);
    mensaje["nombre"] = nivel.nombre;
    mensaje["creador"] = "";
    mensaje["puntaje"] = htonl((uint32_t) nivel.puntaje);
    mensaje["max jugadores"] = htonl((uint32_t) nivel.cant_jugadores_max);
    mensaje["jugadores"] = htonl((uint32_t) jugadores);

    std::string envio = escritor.write(mensaje);

    enviarMsjPrefijo(sockfd, envio.c_str(), envio.length());
}

int SalaPartidas::crearPartida(int& cant_partidas){
    char accion = AVANZAR;
    int i = -1;

    while(accion != ELEGIR && accion != VOLVER) {
        (accion == AVANZAR) ? i++ : i--;

        if (i == -1)
            i = niveles->size() -1;
        else if (i == niveles->size())
            i = 0;
        enviarNivel(*(nuevo_usuario->sockets->enviar_cli), *((*niveles)[i]));

        int res = -1;
        while(res == -1){
            res = nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
            if(res == 0) return CONEXION_ABORTADA;
        }
    }
    if(accion == VOLVER) return -1;

    partida_t* nueva_partida = new partida_t;
    nueva_partida->creador = nuevo_usuario->nombre;
    nueva_partida->nivel = (*niveles)[i];
    nueva_partida->jugadores = new vector<usuario_t*>();
    nueva_partida->jugadores->push_back(nuevo_usuario);

    mutex_partidas->bloquear();
    (*partidas)[cant_partidas++] = nueva_partida;
    mutex_partidas->desbloquear();
    return (cant_partidas -1);
}

void enviarNuevoJugador(Socket& sockfd, partida_t* partida){
    enviarNivel(sockfd, *(partida->nivel), partida->creador, partida->jugadores->size() +1);
}

int SalaPartidas::unirsePartida(){
    char accion = AVANZAR;
    map<int,partida_t*>::iterator it = partidas->begin();

    while(accion != ELEGIR && accion!= VOLVER) {
        if(accion == AVANZAR){
            if(it == partidas->end()) it = partidas->begin();
            ++it;
        } else {
            if(it == partidas->begin()) it = partidas->end();
            --it;
        }
        string* creador = it->second->creador;
        enviarNivel(*(nuevo_usuario->sockets->enviar_cli), *(it->second->nivel), creador, it->second->jugadores->size());

        int res = -1;
        while(res == -1){
            res = nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
            if(res == 0) return CONEXION_ABORTADA;
        }
    }
    if(accion == VOLVER) return -1;
    it->second->jugadores->push_back(nuevo_usuario);
    enviarNuevoJugador(*(nuevo_usuario->sockets->enviar_cli), it->second);

    return it->first;
}

bool SalaPartidas::esperarInicio(int nro_partida){
    char accion = ' ';
    int res = -1;
    while(res == -1){
        res = nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
        if(res == 0) return CONEXION_ABORTADA;
    }

    if(accion == VOLVER) return false;
    return true;
}

bool SalaPartidas::iniciarPartida(int nro_partida){
    char accion = ' ';
    int res = -1;
    while(accion != INICIAR && accion != VOLVER && res == -1){
        res = nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
        if(res == 0) return CONEXION_ABORTADA;
    }
    if(accion == VOLVER) return 0;
    return 1;
}

