#ifndef CONJUNTOJUGADORES_H
#define CONJUNTOJUGADORES_H

#include <vector>
#include "Jugador.h"
#include "Jugada.h"
#include "../libs/TDA/socket/socket.h"
#include "../libs/TDA/thread/thread.h"

class ConjuntoJugadores {
    public:
        ConjuntoJugadores(Mutex* mutex_recibir);
        virtual ~ConjuntoJugadores();
        bool agregar(usuario_t& usuario);
        Jugada* obtenerJugada();
        bool sumarPuntos();
        bool terminarJugada();
        void encolarBorrados(Tablero* tablero);
        int verPuntajeMax();
        void cerrar();
        void enviarTablero(Lista<celda_t*>& celdas);
        void enviarPuntaje(int puntos);
        void enviarGanador(bool hay_ganador);
    protected:
    private:
        Jugador* mejor_jugador;
        int* id_receptor;
        std::vector<Jugador*>* jugadores;
        Mutex* mutex;
};

#endif // CONJUNTOJUGADORES_H
