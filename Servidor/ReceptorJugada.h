#ifndef RECEPTORJUGADA_H
#define RECEPTORJUGADA_H

#include "../libs/TDA/socket/socket.h"
#include "../libs/TDA/thread/thread.h"

typedef struct coordenadas{
    char columna;
    char fila;
} coordenadas_t;

typedef struct parCoordenadas{
    coordenadas_t pos1;
    coordenadas_t pos2;
    char extra;
} par_t;

class ReceptorJugada : public Thread{
    public:
        ReceptorJugada();
        virtual ~ReceptorJugada();
        bool obtenerPar(par_t& nuevo_par);
        void terminar();
        void agregarSocket(Socket *nuevo_socket);
        void agregarMutex(Mutex *nuevo_mutex);
        void agregarIdActual(int* jugador_actual);
        void terminarActual();
    protected:
        virtual void funcion_a_correr();
    private:
        int id_propio;
        static int cant_receptores;
        int* id_jugador;
        par_t *par_recibido;
        Socket* socket;
        Mutex* mutex;
        Mutex* mutex_par_recibido;
        bool seguir;
};

#endif // RECEPTORJUGADA_H
