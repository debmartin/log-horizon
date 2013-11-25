#ifndef EMISORTABLERO_H
#define EMISORTABLERO_H

#include "socket.h"
#include "thread.h"
#include "Lista.h"

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

class EmisorTablero : public Thread {
    public:
        EmisorTablero();
        virtual ~EmisorTablero();
        void agregarSocket(Socket *nuevo_socket);
        void agregarMutex(Mutex *nuevo_mutex);
        void agregarTablero(Lista<celda_t*>* celdas);
    protected:
        virtual void funcion_a_correr();
    private:
        Socket* socket;
        Mutex* mutex;
        Lista<celda_t*>* celdas_tablero;
};

#endif // EMISORTABLERO_H
