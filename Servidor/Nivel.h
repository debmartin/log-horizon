#ifndef NIVEL_H
#define NIVEL_H
#include "Lista.h"
#include "Tablero.h"
#include "Jugador.h"
#include "ConjuntoJugadores.h"
#include <string>

class Nivel{
    public:
        Nivel(Dimension& tam, char** estructura, int max_puntos);
        virtual ~Nivel();
        void agregarJugador(std::string& nombre);
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
