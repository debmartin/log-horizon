#ifndef NIVEL_H
#define NIVEL_H
#include "../libs/TDA/lista/Lista.h"
#include "Tablero.h"
#include "Jugador.h"
#include "ConjuntoJugadores.h"
#include <string>

class Nivel{
    public:
        Nivel(std::istream* arch_estructura, std::istream* arch_probabilidades, int max_puntos);
        virtual ~Nivel();
        void agregarJugador(usuario_t& usuario);
        void imprimirTablero();
        void jugar();
        void cerrarJugador();
        void enviarTablero();
    protected:
    private:
        int puntaje_objetivo;
        Tablero* tablero;
        Mutex* mutex_recibir;
        ConjuntoJugadores* jugadores;
};

#endif // NIVEL_H
