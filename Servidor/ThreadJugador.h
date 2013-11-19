#ifndef THREADJUGADOR_H
#define THREADJUGADOR_H

#include "thread.h"
#include <unistd.h>
#include "Jugador.h"

class ThreadJugador : public Thread {
    public:
        ThreadJugador(Jugador* nuevo_jugador);
        virtual ~ThreadJugador();
    protected:
        virtual void funcion_a_correr();
    private:
        Jugador* jugador;
        Mutex* procesando_mutex;
};

#endif // THREADJUGADOR_H
