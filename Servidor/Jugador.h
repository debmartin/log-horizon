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


class Jugador{
    public:
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

#endif // JUGADOR_H
