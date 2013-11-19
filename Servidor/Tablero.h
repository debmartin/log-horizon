#ifndef TABLERO_H
#define TABLERO_H

#include "Matriz.h"
#include "Celda.h"
#include "Lista.h"
#include "Jugada.h"

class Tablero : public Matriz<Celda>{
    public:
        Tablero(Dimension& tam, char** estructura);
        virtual ~Tablero();
        void imprimir();
        bool intercambiar(Jugada* nueva_jugada);
        bool estabilizar();
        bool hayMovimientos();
    protected:
        void borrarLinea(Dimension& inicio, Dimension& fin, bool por_caida);
        int borrarSegmentosCol(Dimension& dest, Dimension& origen, bool actual_inclusive = true);
        int borrarColumna(Dimension& dest, Dimension& origen);
        int borrarFila(Dimension& inicio, Dimension& fin);
        void estabilizarJugada();
        void estabilizar(Dimension& pos1, Dimension& pos2, bool por_caida);
        bool hayMovimiento(Dimension& pos);
        int buscarConfEspecial(Dimension& pos, int orientacion, Dimension& inicial, Dimension& final);
        void explosionEstrella();
        bool reemplazarOriginal(int len_linea, Dimension& pos, int orientacion);
    private:
        Lista<Dimension*>* modificados;
        Jugada* jugada_en_curso;
};

#endif // TABLERO_H
