#ifndef TABLERO_H
#define TABLERO_H

#include "Matriz.h"
#include "Celda.h"
#include "../libs/TDA/lista/Lista.h"
#include "Jugada.h"

/* Tablero de Candy Crush */
class Tablero : public Matriz<Celda>{
    public:
        /* Crea un tablero.
         * Pre: tam es el tamanio del tablero (filas, columnas). estructura es
         * un vector de cadenas de caracteres '0' o '1' que representan la
         * estructura de la matriz, siendo '0' un vacío y '1' una celda válida.
         * Cada posicion del vector estructura representa una columna de la
         * matriz.
         * Post: El tablero fue creado con el tamanio y estructura dadas, y se
         * lleno con Celdas. */
        Tablero(Dimension& tam, char** estructura);
        /* Destruye el tablero.
         * Pre: El tablero fue creado.
         * Post: El tablero fue destruido. */
        virtual ~Tablero();
        /* Devuelve el tamanio del tablero.
         * Pre: El tablero fue creado.*/
        Dimension getTamanio() { return *tamanio; }
        //BORRAR
        void imprimir();
        /* Intercambia dos posiciones.
         * Pre: Se hizo estabilizar luego de cualquier llamada anterior a
         * intercambiar. nueva_jugada fue creada. Sus posiciones son adyacentes
         * y se encuentran dentro del rango del tablero.
         * Post: Si alguna de las posiciones de la jugada provoca una
         * configuracion especial de tres o más elementos del mismo color, se
         * eliminan, se suman los puntos que correspondan a la jugada, se le
         * agregan las posiciones de las fichas borradas, y se realiza la caída
         * de las fichas superiores en el tablero. Si alguna es una estrella,
         * tambien se considera configuracion especial. Si las dos posiciones
         * en la jugada provocan una configuracion especial, se realiza el
         * mismo proceso para ambas. Si ninguna de las dos provoca una
         * configuracion especial, no son adyacentes o no se hizo estabilizar,
         * devuelve false y no realiza ningun cambio. Devuelve true en caso
         * contrario. */
        bool intercambiar(Jugada* nueva_jugada);
        /* Elimina todas las configuraciones especiales del tablero.
         * Pre: Se realizo intercambiar inmediatamente antes.
         * Post: Borra todas las configuraciones especiales, excepto que haya
         * una estrella, sumando los puntos a la jugada dada anteriormente, y
         * agregando las posiciones borradas a la misma. El tablero queda en un
         * estado estable. */
        bool estabilizar();
        /* Determina si hay movimientos posibles que logren una configuracion
         * especial.
         * Pre: El tablero fue creado.
         * Post: Devuelve true si el hay movimientos posibles, y false en caso
         * contrario. */
        bool hayMovimientos();
    protected:
        /* Borra la linea indicada.
         * Pre: inicio y fin son posiciones validas del tablero, no vacias.
         * por_caida debe ser true si el borrado se debe a la generacion de una
         * configuracion especial debido a la caida de fichas, false en caso
         * contrario. Ambas posiciones deben estar en la misma fila o la misma
         * columna. La otra coordenada debe darse, de menor a mayor en inicio y
         * fin. Ambas dimensiones pertenecen al segmento a ser borrado.
         * Post: Se borraron todas las celdas entre inicio y fin incluyendolas.
         * Se sumaron los puntajes correspondientes a la jugada que se ingreso
         * en intercambiar. Si es por caida, el puntaje es doble. Se agregaron
         * las posiciones borradas a la jugada. */
        void borrarLinea(Dimension& inicio, Dimension& fin, bool por_caida);
        /* Borra el segmento de columna indicada.
         * Pre: origen y dest son posiciones validas del tablero, no vacias.
         * Ambas posiciones deben estar en la misma columna. La otra
         * coordenada debe darse, de menor a mayor en origen y dest. Ambas
         * dimensiones pertenecen al segmento a ser borrado. No hay celdas
         * vacias en el segmento. Si actual_inclusive es true, se borra el
         * segmento que incluye a las dimensiones dadas.
         * Post: Se borraron todas las celdas entre origen y dest
         * incluyendolas. Se sumaron los puntajes correspondientes a la jugada
         * que se ingreso en intercambiar. Se realizó la caida. */
        int borrarSegmentosCol(Dimension& dest, Dimension& origen, \
                               bool dest_inclusive = true);
        /* Borra el segmento de columna indicada.
         * Pre: origen y dest son posiciones validas del tablero, no vacias.
         * Las coordenadas deben darse, de menor a mayor en origen y dest. Ambas
         * dimensiones pertenecen al segmento a ser borrado. No hay celdas
         * vacias en el segmento.
         * Post: Se borraron todas las celdas entre origen y dest
         * incluyendolas. Se sumaron los puntajes correspondientes a la jugada
         * que se ingreso en intercambiar. Se realizó la caida. */
        int borrarColumna(Dimension& dest, Dimension& origen);
        /* Borra el segmento de fila indicada.
         * Pre: inicio y fin son posiciones validas del tablero, no vacias.
         * Las coordenadas deben darse, de menor a mayor en inicio y fin. Ambas
         * dimensiones pertenecen al segmento a ser borrado. No hay celdas
         * vacias en el segmento.
         * Post: Se borraron todas las celdas entre origen y dest
         * incluyendolas. Se sumaron los puntajes correspondientes a la jugada
         * que se ingreso en intercambiar. Se realizó la caida. */
        int borrarFila(Dimension& inicio, Dimension& fin);
        /* Borra las configuraciones especiales de la ultima jugada.
         * Pre: Se intercambiaron las fichas de la jugada.
         * Post: */
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
