#ifndef DATOSJUGADOR_H
#define DATOSJUGADOR_H

#include "Jugador.h"
#include "ThreadJugador.h"
#include "socket.h"
#include <string>

typedef struct sock_jugador{
    Socket* enviar;
    Socket* enviar_cli;
    Socket* recibir;
    Socket* recibir_cli;
} sockets_jugador_t;



class DatosJugador {
    public:
        DatosJugador(int enviar, int recibir);
        virtual ~DatosJugador();
        int prepararSocketEnviar();
        int prepararSocketRecibir();
        void crearJugador(std::string& nombre);
    protected:
        int prepararSocket(int tipo, int puerto);
    private:
        int puerto_enviar;
        int puerto_recibir;
        Jugador* jugador;
        sockets_jugador_t* sockets;
        ThreadJugador* thread;
};

#endif // DATOSJUGADOR_H
