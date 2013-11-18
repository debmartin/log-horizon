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
        bool intercambiar(Jugada* nueva_jugada);
        void estabilizar();
        bool hayMovimientos();
    protected:
        int borrarLinea(Dimension& inicio, Dimension& fin);
        int borrarColumna(int col);
        int borrarFila(int fila);
        int borrarColumna(Dimension& dest, Dimension& origen, bool borrando);
        int borrarFila(Dimension& inicio, Dimension& fin, bool borrando);
        void estabilizar(Dimension& una, Dimension& otra);
        bool hayMovimiento(Dimension& pos);
        int buscarConfEspecial(Dimension& pos, int orientacion, Dimension& inicial, Dimension& final);
        int explosionEstrella(Dimension& una, Dimension& otra);
        bool reemplazarOriginal(int len_linea, Dimension& pos, int orientacion);
    private:
        Lista<Dimension*>* borrados;
        Lista<Dimension*>* modificados;
        Jugada* jugada_en_curso;
};

#endif // TABLERO_H
