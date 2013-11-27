#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "../libs/TDA/socket/socket.h"
#include <exception>
#include <string>

#define PUERTO_ESCUCHA 8000

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
    int cant_jugadores;
} nivel_t;



class ServidorCrearSocket : public std::exception{};

class Servidor
{
    public:
        Servidor();
        virtual ~Servidor();
        void recibirConexion(sockets_jugador_t& sockets);
        int asignarPuerto(Socket& sockfd);
        void enviarPuertos(sockets_jugador_t& sockets);
        bool aceptarSubConexiones(sockets_jugador_t& sockets);
        void recibirUsuarioContrasenia(usuario_t& nuevo_usuario);
    protected:
    private:
        int proximo_puerto;
        Socket* cliente_actual;
        Socket* socket_escucha;
        usuario_t* nuevo_usuario;
        vector<nivel_t*> niveles;
        vector<nivel_t*> partidas;
};

#endif // SERVIDOR_H
