#include "Tablero.h"
#define ESPACIO '0'

Tablero::Tablero(Dimension& tam, char** estructura)
    : Matriz<Celda>(tam), jugada_en_curso(NULL){
    modificados = new Lista<Dimension*>(true);
    for(int i=0; i < tam.y(); i++){
        for(int j=0; j < tam.x(); j++){
            if(estructura[i][j] == ESPACIO){
                (*this)[j][i].setTipo(VACIO);
                (*this)[j][i].setColor(VACIO);
            }
        }
    }
}

Tablero::~Tablero(){
    delete modificados;
}

bool Tablero::intercambiar(Jugada* nueva){
    if(jugada_en_curso) return false;
    jugada_en_curso = nueva;
    Dimension pos1(nueva->posicion1()), pos2(nueva->posicion2());


    if (! sonAdyacentes(pos1, pos2)) return false;
    if ((! hayMovimiento(pos1)) && (! hayMovimiento(pos2)))
        return false;

    Matriz::intercambiar(pos1, pos2);
    estabilizarJugada();
    return true;
}

void Tablero::estabilizar(Dimension& pos1, Dimension& pos2, bool por_caida){
    int orientacion = FILA;
    bool ultimo = false;
    Dimension actual(pos1);

    while(!ultimo){
        Dimension inicial(actual), final(actual);
        int cant = buscarConfEspecial(actual, orientacion, inicial, final);
        if(cant >= MIN_LINEA){
            borrarLinea(inicial, final, por_caida);
            reemplazarOriginal(cant, actual, orientacion);
        }
        if(orientacion == FILA){
            orientacion = COLUMNA;
        } else if (actual == pos1) {
            actual = pos2;
            orientacion = FILA;
        } else {
            ultimo = true;
        }
    }
}

void Tablero::estabilizarJugada(){
    Dimension pos1(jugada_en_curso->posicion1()), pos2(jugada_en_curso->posicion2());
    if((*this)[pos1].esEstrella() || (*this)[pos2].esEstrella()){
        explosionEstrella();
        return;
    }
    estabilizar(pos1, pos2, false);
}

int Tablero::borrarSegmentosCol(Dimension& dest, Dimension& origen, bool dest_inclusive){
    bool en_segmento = false, contar_segmento = true;
    int cant = 0;
    Dimension primero(dest), ultimo(origen), no_vacia(dest);

    for (int i = dest.y(); i <= origen.y(); i++){
        Dimension actual(origen.x(), i);

        if(! contar_segmento) continue;

        if(actual == dest && !dest_inclusive)
            contar_segmento = false;

        if((*this)[actual].esVacia() && en_segmento) {
            ultimo = no_vacia;
            en_segmento = false;
            contar_segmento = true;
            cant += borrarColumna(primero, ultimo);
        } else if (! (*this)[actual].esVacia()) {
            no_vacia = actual;
            if (! en_segmento){
                primero = actual;
                en_segmento = true;
            }
        }
    }
    return cant;
}

int Tablero::borrarColumna(Dimension& dest, Dimension& origen){
    if(! dest.esValida() || ! origen.esValida()) return 0;

    int cant = 0;
    int max = dest.x();
    bool hay_barv = false, termino_segmento = false;
    for(int i=0; i <= max; i++){
        Dimension actual(dest.x() -i, dest.y());
        Dimension superior(origen.x() -i-1, origen.y());
        char tipo = (*this)[actual].getTipo();

        if (tipo == MINIBARH){
            Dimension principio(actual.x(), 0);
            Dimension final(actual.x(), tamanio->y()-1);
            cant += borrarFila(principio, final);
        } else if (tipo == MINIBARV) {
            hay_barv = true;
        } else {
            if(superior.esValida() && (*this)[superior].esVacia())
                termino_segmento = true;
            if ((*this)[actual].esVacia())
                break;
            if (superior.esValida() && ! termino_segmento)
                (*this)[actual] = (*this)[superior];
            else
                (*this)[actual].rellenar();
            cant++;
            *modificados += new Dimension(actual);
        }
    }
    if(hay_barv){
        Dimension principio(tamanio->x() -1, origen.y());
        Dimension fin(tamanio->x() - dest.x() + origen.x() -1, dest.y());
        cant += borrarSegmentosCol(principio, fin);

        Dimension primero(0, dest.y());
        Dimension ultimo(tamanio->x() -1, dest.y());
        cant += borrarSegmentosCol(primero, ultimo, false);
    }
    return cant;
}

int Tablero::borrarFila(Dimension& inicio, Dimension& fin){
    int max = fin.y() - inicio.y(), cant = 0;
    for(int i=0; i<=max; i++){

        Dimension actual(inicio.x(), inicio.y()+i);

        char tipo = (*this)[actual].getTipo();

        if (tipo == MINIBARH){
            cant += borrarColumna(actual, actual);
            Dimension principio(actual.x(), 0);
            Dimension fin(actual.x(), inicio.y() -1);
            if (principio != inicio)
                cant += borrarFila(principio, fin);
            max = tamanio->y() -1;
        } else if (tipo == MINIBARV) {
            Dimension origen(-1, actual.y());
            Dimension dest(tamanio->x() -1, actual.y());
            cant += borrarSegmentosCol(dest, origen);
        } else if (tipo == VACIO) {
            continue;
        } else {
            cant += borrarColumna(actual, actual);
        }
    }
    return cant;
}

void Tablero::borrarLinea(Dimension& inicio, Dimension& fin, bool por_caida){
    int cant = 0;
    if (inicio.x() == fin.x()){
        cant += borrarFila(inicio, fin);
    } else {
        cant += borrarColumna(fin, inicio);
    }
    jugada_en_curso->sumarPuntos(cant, false, por_caida);
}

int Tablero::buscarConfEspecial(Dimension& pos, int orientacion, Dimension& inicial, Dimension& final){
    int fila = pos.x(), col = pos.y();
    int inicio = (orientacion == FILA) ? ((col-2 >= 0) ? col-2 : 0) : ((fila-2  >= 0) ? fila-2 : 0);
    int fin = (orientacion == FILA) ? ((col+2 < tamanio->y()) ? col+2 : tamanio->y() -1) : ((fila+2  < tamanio->x()) ? fila+2 : tamanio->x() -1);
    int i = inicio-1, cant = 0;
    int& fila_n = (orientacion == FILA) ? fila : i;
    int& col_n = (orientacion == FILA) ? i : col;

    char color_pos = (*this)[pos].getColor();
    bool final_marcado = false;
    Dimension pos_ant(fila_n, col_n);
    i++;
    while(i <= fin){
        Dimension pos_actual(fila_n, col_n);

        if ((*this)[pos_actual].getColor() != color_pos){
            if (cant >= MIN_LINEA){
                final = pos_ant;
                final_marcado = true;

            } else {
                cant = 0;
            }
            i++;
            continue;
        }
        if (cant == 0)
            inicial = pos_actual;
        pos_ant = pos_actual;
        cant++;
        i++;
    }
    if (! final_marcado){
        i--;
        Dimension aux(fila_n, col_n);
        final = aux;
    }
    return cant;
}

void Tablero::explosionEstrella(){
    Dimension pos1(jugada_en_curso->posicion1()), pos2(jugada_en_curso->posicion2());
    Dimension pos_no_est = ((*this)[pos1].esEstrella()) ? pos2 : pos1;
    Celda& celda_no_est = (*this)[pos_no_est];
    char color_orig = celda_no_est.getColor();
    char tipo_orig = celda_no_est.getTipo();
    int cant = 1;

    if(tipo_orig == BUTTON) {
        Dimension pos_est = ((*this)[pos1].esEstrella()) ? pos1 : pos2;
        borrarColumna(pos_est, pos_est);
    }

    for(int i=0; i < tamanio->y(); i++){
        if (tipo_orig == ESTRELLA){
            Dimension inicio(tamanio->x() -1, i);
            Dimension fin(0, i);
            cant += borrarSegmentosCol(inicio, fin);
            continue;
        }
        for(int j=0; j < tamanio->x(); j++){
            Dimension pos_actual(j, i);

            if((*this)[pos_actual].getColor() == color_orig) {
                if(tipo_orig == MINIBARH || tipo_orig == MINIBARV){
                    Dimension inicio1(pos_no_est.x(), 0);
                    Dimension fin1(pos_no_est.x(), tamanio->y() -1);
                    cant += borrarFila(inicio1, fin1) -1;

                    Dimension inicio2(tamanio->x() -1, pos_no_est.y());
                    Dimension fin2(0, pos_no_est.y());
                    cant += borrarSegmentosCol(inicio2, fin2);
                } else {
                    cant += borrarColumna(pos_actual, pos_actual);
                }
            }
        }
    }
    jugada_en_curso->sumarPuntos(cant, true);
}

bool Tablero::reemplazarOriginal(int len_linea, Dimension& pos, int orientacion){
    if (len_linea <= 4) {
        return false;
    } else if (len_linea == 4){
        if (orientacion == FILA)
            (*this)[pos].setTipo(MINIBARH);
        else
            (*this)[pos].setTipo(MINIBARV);
    } else {
        (*this)[pos].setTipo(ESTRELLA);
        (*this)[pos].setColor(ESTRELLA);
    }
    return true;
}

bool enRango(Dimension& pos, Dimension& tam){
    if(!pos.esValida()) return false;
    return ((pos.x() < tam.x()) && (pos.y() < tam.y()));
}

bool Tablero::hayMovimiento(Dimension& pos){
    if ((*this)[pos].esEstrella())
        return true;

    Dimension aux1(pos), aux2(pos);

    for(int i = pos.x()-1; i <= pos.x()+1; i++){
        for(int j = (i==pos.x()) ? pos.y()-1 : pos.y(); (i==pos.x()) ? (j <= pos.y()+1) : (j == pos.y()); j+=2){
            Dimension adyacente(i, j);
            if(!enRango(adyacente, *tamanio)) continue;
            if(! Matriz::intercambiar(pos, adyacente))
                continue;

            int orientacion = FILA;
            bool ultimo = false;

            while(!ultimo){
                int cant = buscarConfEspecial(pos, orientacion, aux1, aux2);
                if(cant >= MIN_LINEA){
                    Matriz::intercambiar(pos, adyacente);
                    return true;
                }
                if(orientacion == FILA){
                    orientacion = COLUMNA;
                } else {
                    ultimo = true;
                }
            }
            Matriz::intercambiar(pos, adyacente);
        }
    }
    return false;
}

bool Tablero::hayMovimientos(){
    for(int i=0; i < tamanio->x(); i++){
        for(int j=0; i < tamanio->y(); j++){
            Dimension actual(i,j);
            if (hayMovimiento(actual))
                return true;
        }
    }
    return false;
}

bool Tablero::estabilizar(){
    if(! jugada_en_curso) return false;
    while(! modificados->esVacia()){
        Dimension* pos = modificados->borrarPrimero();

        jugada_en_curso->agregarBorrado(pos);

        Dimension ady1(pos->x()-1, pos->y()), ady2(pos->x()+1, pos->y());
        Dimension ady3(pos->x(), pos->y()-1), ady4(pos->x(), pos->y()+1);
        Dimension vector[] = {ady1, ady2, ady3, ady4};
        for(int i=0; i<4; i++){
            Dimension adyacente(vector[i]);
            if(! enRango(adyacente, *tamanio)) continue;

            int orientacion = FILA;
            bool ultimo = false;

            while(!ultimo){
                estabilizar(*pos, adyacente, true);
                if(orientacion == FILA)
                    orientacion = COLUMNA;
                else
                    ultimo = true;
            }
        }
    }
    jugada_en_curso = NULL;
    return true;
}

void Tablero::imprimir(){
    std::cout << "      ";
    for(int i=0; i<tamanio->y(); i++)
        std::cout << i << "     ";
    std::cout << '\n' << '\n';

    for(int i=0; i<tamanio->x(); i++){
        std::cout << i << "     ";
        for(int j=0; j<tamanio->y(); j++){
            Celda salida = (*this)[i][j];
            std::cout << '(' << salida.getColor() << ',' << salida.getTipo() << ')';
                std::cout << "     ";
        }
        std::cout << '\n';
    }
    std::cout << '\n' << '\n';
}
