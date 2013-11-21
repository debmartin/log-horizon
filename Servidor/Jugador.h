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


class Jugador{
    public:
        Jugador(std::string& nombre, int enviar, int recibir);
        virtual ~Jugador();
        void iniciar();
        int getPuntaje() { return puntaje; }
        bool sumarPuntos();
        Jugada* obtenerJugada();
        bool terminarJugada();
        bool encolarBorrados(Tablero* tablero);
        int prepararSocketRecibir();
        int prepararSocketEnviar();
        void enviarBorrados();
        void cerrarJugador();
    protected:
        int prepararSocket(int tipo, int puerto);
        bool recibirPar();
    private:
        int puerto_enviar;
        int puerto_recibir;
        std::string* id;
        int puntaje;
        Jugada* jugada_actual;
        EmisorResultados* emisor;
        Mutex* mutex;
        sockets_jugador_t* sockets;
};
#endif // JUGADOR_H
