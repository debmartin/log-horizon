#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <sstream>
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

typedef struct celda_tablero{
    char col;
    char fila;
    char tipo;
    char color;
} celda_t;

typedef struct msj_celda{
    char tipo;
    celda_t celda;
} msj_celda_t;

typedef struct msj_puntos{
    char tipo;
    char indice1;
    char indice2;
    char indice3;
    char indice4;
} msj_puntos_t;



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
        void enviarPuntaje();
        void enviarCelda(celda_t& celda);
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
