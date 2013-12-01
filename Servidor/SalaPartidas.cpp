#include "SalaPartidas.h"

#define TAM_UINT32 sizeof(uint32_t)
#define OK '0'

using std::vector;
using std::map;

SalaPartidas::SalaPartidas(usuario_t* usuario, vector<nivel_t*>* v_niveles, map<int,partida_t*>* v_partidas) :
    nuevo_usuario(usuario), niveles(v_niveles), partidas(v_partidas){
}

SalaPartidas::~SalaPartidas(){
}


int SalaPartidas::definirTipoPartida(){
    char tipo_partida = ' ';
    nuevo_usuario->sockets->recibir_cli->recibir(&tipo_partida, sizeof(char));
    return (tipo_partida - '0');
}

void enviarNivel(Socket& sockfd, nivel_t& nivel){
    char msj = OK;
    sockfd.enviar(&msj, sizeof(char));        //SIGUE VALIENDO?
    enviarMsjPrefijo(sockfd, nivel.nombre->c_str(), sizeof(nivel.nombre->c_str()));

    uint32_t puntos = htonl((uint32_t) nivel.puntaje);
    sockfd.enviar(&puntos, TAM_UINT32);

    uint32_t cant_jug = htonl((uint32_t) nivel.cant_jugadores_max);
    sockfd.enviar(&cant_jug, TAM_UINT32);
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

        nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
    }
    if(accion == VOLVER) return -1;

    partida_t* nueva_partida = new partida_t;
    nueva_partida->creador = nuevo_usuario->nombre;
    nueva_partida->nivel = (*niveles)[i];
    nueva_partida->jugadores = new vector<usuario_t*>();
    nueva_partida->jugadores->push_back(nuevo_usuario);

    partidas->insert(std::pair<int, partida_t*> (cant_partidas++, nueva_partida));
    return (cant_partidas -1);
}

void enviarNuevoJugador(partida_t* partida){
    int uno = 1;
    uint32_t cant = htonl((uint32_t) uno);
    for(vector<usuario_t*>::iterator it = partida->jugadores->begin(); it != partida->jugadores->end(); ++it){
        enviarNivel(*((*it)->sockets->enviar_cli), *(partida->nivel));
        (*it)->sockets->enviar_cli->enviar(&cant, sizeof(uint32_t));
    }
}       //ENVIO SOLO EL NUMERO O TODO?

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

        enviarNivel(*(nuevo_usuario->sockets->enviar_cli), *(it->second->nivel));
        const char* creador = it->second->creador->c_str();
        enviarMsjPrefijo(*(nuevo_usuario->sockets->enviar_cli), creador, sizeof(creador));
        uint32_t cant_jug = htonl((uint32_t) it->second->jugadores->size());
        nuevo_usuario->sockets->enviar_cli->enviar(&cant_jug, TAM_UINT32);

        nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
    }
    if(accion == VOLVER) return -1;
    it->second->jugadores->push_back(nuevo_usuario);
    enviarNuevoJugador(it->second);

    return it->first;
}

bool SalaPartidas::esperarInicio(int nro_partida){
    char accion = ' ';
    if(accion != VOLVER)       //CORREGIR
        nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
    if(accion == VOLVER) return false;
    return true;
}

bool SalaPartidas::iniciarPartida(int nro_partida){
    char accion = ' ';
    while(accion != INICIAR && accion != VOLVER)
        nuevo_usuario->sockets->recibir_cli->recibir(&accion, sizeof(char));
    if(accion == VOLVER) return false;
    return true;
}

