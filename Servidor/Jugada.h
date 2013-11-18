#ifndef JUGADA_H
#define JUGADA_H

#include "dimension.h"
#include "Constantes.h"

class Jugada {
    public:
        Jugada(Dimension& principio, Dimension& fin);
        virtual ~Jugada();
        bool sumarPuntos(int cant_borrados, bool con_estrella = false, bool por_caida = false);
        int verPuntos();
        Dimension posicion1();
        Dimension posicion2();
    protected:
    private:
        int puntos;
        Dimension* pos1;
        Dimension* pos2;
};

#endif // JUGADA_H
