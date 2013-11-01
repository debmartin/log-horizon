#ifndef JUEGO_H
#define JUEGO_H

#include "dimension.h"
#include "Matriz.h"
#include "Cola.h"
#include "Lista.h"

class Juego {
    public:
        Juego();
        virtual ~Juego();
        int getPuntaje() { return puntaje; }
        void imprimirTablero();
        int realizarJugada(Dimension& una, Dimension& otra);
        bool hayMovimientos();
        int getLimite() { return puntaje_nivel; }
    protected:
        bool borrarElementos(Dimension& original, bool modificado);
        int buscarLinea(Dimension& pos, int orientacion);
        bool reemplazarOriginal(int len_linea, Dimension& pos, int orientacion);
        bool procesarLinea(Dimension& pos, int orientacion);
        void contarPuntos(int largo_linea);
        bool intentarJugada(Dimension& una, Dimension& otra);
        void rellenarCelda(Dimension& pos);
        int borrarLinea(Dimension& pos, int orient_bar, int orientacion);
        int borrarCelda(Dimension* pos, Celda& celda_no_est, int& cant);
        int procesarEstrella(Dimension& una, Dimension& otra);
    private:
        const int puntaje_nivel;
        int puntaje;
        bool estable;
        Matriz* matriz;
        Lista<Dimension*>* borrados;
};

#endif // JUEGO_H
