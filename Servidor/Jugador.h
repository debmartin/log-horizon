#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>
#include "Jugada.h"

class Jugador{
    public:
        Jugador(std::string& nombre);
        virtual ~Jugador();
        int getPuntaje() { return puntaje; }
        bool sumarPuntos();
        Jugada* obtenerJugada();
        void terminarJugada();
    protected:
    private:
        std::string* id;
        int puntaje;
        Jugada* jugada_actual;
};

#endif // JUGADOR_H
