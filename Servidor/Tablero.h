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
        bool intercambiar(Dimension& una, Dimension& otra);
        int buscarConfEspecial(Dimension& pos, int orientacion, Dimension& inicial, Dimension& final);
        int explosionEstrella(Dimension& una, Dimension& otra);
        bool reemplazarOriginal(int len_linea, Dimension& pos, int orientacion);
        int borrarLinea(Dimension& inicio, Dimension& fin);
    protected:
        bool contarPuntos(int largo_linea, bool conEstrella);
        int borrarColumna(int col);
        int borrarFila(int fila);
        int borrarColumna(Dimension& dest, Dimension& origen, bool borrando);
        int borrarFila(Dimension& inicio, Dimension& fin, bool borrando);
        void estabilizar();
        void estabilizar(Dimension& una, Dimension& otra);
        bool hayMovimiento(Dimension& pos);
        bool hayMovimientos();
    private:
        Lista<Dimension*>* borrados;
        Lista<Dimension*>* modificados;
        bool* mov_columna;
};

#endif // TABLERO_H
