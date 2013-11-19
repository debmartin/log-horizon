#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "Jugada.h"
#include "socket.h"
#include "emisor_resultados.h"
#include "Tablero.h"

typedef struct coordenadas{
    char columna;
    char fila;
} coordenadas_t;

typedef struct parCoordenadas{
    coordenadas_t pos1;
    coordenadas_t pos2;
    char extra;
} par_t;

typedef struct socks{
    Socket* enviar;
    Socket* enviar_cli;
    Socket* recibir;
    Socket* recibir_cli;
} sockets_jugador_t;

class DatosJugador;

class Jugador{
    public:
        friend class DatosJugador;

        Jugador(std::string& nombre, Socket* enviar, Socket* recibir);
        virtual ~Jugador();
        int getPuntaje() { return puntaje; }
        bool sumarPuntos();
        Jugada* obtenerJugada();
        bool terminarJugada();
        bool recibirPar();
        bool enviarBorrados(Tablero* tablero);
    protected:
    private:
        std::string* id;
        int puntaje;
        Jugada* jugada_actual;
        Socket* socket_recepcion;
        EmisorResultados* emisor;
        Mutex* mutex;
};

class DatosJugador{
    public:
        DatosJugador(int enviar, int recibir);
        ~DatosJugador();
        void crearJugador(std::string& nombre);
        int prepararSocket(int tipo, int puerto);
        int prepararSocketEnviar();
        int prepararSocketRecibir();
        int getPuntaje();
        bool sumarPuntos();
        Jugada* obtenerJugada();
        bool terminarJugada();
        bool recibirPar();
        bool encolarBorrados(Tablero* tablero);
        void enviarBorrados();
        void cerrarJugador();
    private:
        int puerto_enviar;
        int puerto_recibir;
        Jugador* jugador;
        ThreadJugador* thread;
        sockets_jugador_t* sockets;
};
#endif // JUGADOR_H
