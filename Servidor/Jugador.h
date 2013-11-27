#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include <sstream>
#include "../libs/TDA/socket/socket.h"
#include "../libs/TDA/thread/thread.h"
#include "Jugada.h"
#include "emisor_resultados.h"
#include "Tablero.h"
#include "ReceptorJugada.h"
#include "EmisorTablero.h"


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
        int getPuntaje() { return puntaje; }
        std::string Id() { return *id; }
        bool sumarPuntos();
        Jugada* obtenerJugada();
        bool terminarJugada();
        bool encolarBorrados(Tablero* tablero);
        int prepararSocketRecibir();
        int prepararSocketEnviar();
        void enviarBorrados();
        void cerrar();
        void enviarPuntaje(int puntos);
        void enviarTablero(Lista<celda_t*>& celdas);
        void agregarMutex(Mutex* mutex_recibir);
        void agregarIdActual(int* id_jugador);
        void esperarJugada();
        void terminarEmisionTablero();
        void enviarMensaje(void* mensaje, int len);
    protected:
        int prepararSocket(int tipo, int puerto);
        bool recibirPar();
    private:
        int puerto_enviar;
        int puerto_recibir;
        std::string* id;
        int puntaje;
        Jugada* jugada_actual;
        EmisorTablero* emisor_tab;
        EmisorResultados* emisor;
        Mutex* mutex_emisor;
        Mutex* mutex_receptor;
        sockets_jugador_t* sockets;
        ReceptorJugada* receptor;
};
#endif // JUGADOR_H
