#ifndef THREADJUGADOR_H
#define THREADJUGADOR_H

#include "thread.h"
#include <unistd.h>
#include "emisor_resultados.h"

class ThreadJugador : public Thread {
    public:
        ThreadJugador(EmisorResultados* emisor);
        virtual ~ThreadJugador();
    protected:
        virtual void funcion_a_correr();
    private:
        EmisorResultados* emisor_res;
};

#endif // THREADJUGADOR_H
