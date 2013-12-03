#ifndef EMISORTABLERO_H
#define EMISORTABLERO_H

#include "../libs/TDA/socket/socket.h"
#include "../libs/TDA/thread/thread.h"
#include "../libs/TDA/lista/Lista.h"

typedef struct celda_tablero{
    char col;
    char fila;
    char tipo;
    char color;
} celda_t;

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
