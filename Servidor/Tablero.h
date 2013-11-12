#ifndef TABLERO_H
#define TABLERO_H

#include "Matriz.h"
#include "Celda.h"
#include "Lista.h"

class Tablero : public Matriz<Celda>{
    public:
        Tablero(int tam, Lista<Dimension*>* cola_modif);
        virtual ~Tablero();
        void imprimir();
        int intercambiar(Dimension& una, Dimension& otra);
    protected:
        bool intentarJugada(Dimension& una, Dimension& otra);
        int borrarColumna(int col);
        int borrarFila(int fila);
        int borrarColumna(Dimension& dest, Dimension& origen, bool borrando);
        int borrarFila(Dimension& inicio, Dimension& fin, bool borrando);
        int borrarLinea(Dimension& inicio, Dimension& fin);
        int buscarConfEspecial(Dimension& pos, int orientacion);
        int buscarConfEspecial(Dimension& una, Dimension& otra);
        bool reemplazarOriginal(int len_linea, Dimension& pos, int orientacion);
        bool hayMovimientosCol(int col);
        void verMovimientos();
        bool hayMovimientos();
    private:
        Lista<Dimension*>* modificados;
        bool* mov_columna;
};

#endif // TABLERO_H
