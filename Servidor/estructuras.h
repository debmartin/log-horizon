#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include <vector>
#include "../libs/TDA/socket/socket.h"

typedef struct _socks{
    Socket* enviar;
    Socket* enviar_cli;
    Socket* recibir;
    Socket* recibir_cli;
} sockets_jugador_t;


typedef struct _usuario{
    std::string* nombre;
    std::string* contrasenia;
    sockets_jugador_t* sockets;
} usuario_t;

typedef struct _nivel{
    std::string* nombre;
    int puntaje;
    int cant_jugadores_max;
    std::string* archivo_tablero;
    std::string* archivo_probabilidades;
} nivel_t;

typedef struct _partida {
    std::string* creador;
    nivel_t* nivel;
    std::vector<usuario_t*>* jugadores;
} partida_t;

#endif // ESTRUCTURAS_H_INCLUDED
