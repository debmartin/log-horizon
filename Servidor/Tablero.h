#ifndef TABLERO_H
#define TABLERO_H

#include "Matriz.h"
#include "Celda.h"
#include "Lista.h"
#include "Jugada.h"

class Tablero : public Matriz<Celda>{
    public:
        Tablero(int tam, Lista<Dimension*>* cola_modif);
        virtual ~Tablero();
        void imprimir();
        bool intercambiar(Jugada* nueva_jugada);
        bool estabilizar();
        bool hayMovimientos();
    protected:
        void borrarLinea(Dimension& inicio, Dimension& fin, bool por_caida);
        int borrarColumna(int col);
        int borrarFila(int fila);
        int borrarColumna(Dimension& dest, Dimension& origen, bool borrando);
        int borrarFila(Dimension& inicio, Dimension& fin, bool borrando);
        void estabilizarJugada();
        void estabilizar(Dimension& pos1, Dimension& pos2, bool por_caida);
        bool hayMovimiento(Dimension& pos);
        int buscarConfEspecial(Dimension& pos, int orientacion, Dimension& inicial, Dimension& final);
        void explosionEstrella();
        bool reemplazarOriginal(int len_linea, Dimension& pos, int orientacion);
    private:
        Lista<Dimension*>* borrados;
        Lista<Dimension*>* modificados;
        Jugada* jugada_en_curso;
};

#endif // TABLERO_H
