#include "Juego.h"

#define FILA 0
#define COLUMNA 1
#define PUNTAJE_LINEA_3 30
#define PUNTAJE_LINEA_4 80
#define PUNTAJE_LINEA_5 150

#define ROJO 'r'
#define AMARILLO 'y'
#define VERDE 'g'
#define AZUL 'b'
#define VIOLETA 'v'
#define ORO 'o'
#define VACIO 'n'
#define BUTTON 'c'
#define MINIBARH 'h'
#define MINIBARV 'p'
#define ESTRELLA 's'

#define LIMITE 300

Juego::Juego() : puntaje_nivel(LIMITE), puntaje(0), estable(true) {
    matriz = new Matriz();
    borrados = new Lista<Dimension*>(true);
}

Juego::~Juego(){
    delete matriz;
    delete borrados;
}

void Juego::imprimirTablero(){
    matriz->imprimir();
}

int Juego::borrarLinea(Dimension& pos, int orient_bar, int orientacion){
    int fila = pos.getFila(), col = pos.getCol(), contador = 0, tam_matriz = matriz->getTamanio();
    int j = (orientacion == orient_bar) ? ((orientacion == FILA) ? fila : col) : 0;
    int& fila_n = (orient_bar == FILA) ? j : fila;
    int& col_n = (orient_bar == FILA) ? col : j;

    for (; j < tam_matriz; j++) {
        *borrados += new Dimension(fila_n, col_n);
        contador++;
    }
    return contador;
}

int Juego::buscarLinea(Dimension& pos, int orientacion){
    int fila = pos.getFila(), col = pos.getCol();
    int tam_matriz = matriz->getTamanio(), cant = 0, i = 0;
    char color_pos = (*matriz)[pos].getColor();

    int& fila_n = (orientacion == FILA) ? i : fila;
    int& col_n = (orientacion == FILA) ? col : i;

    while (i < tam_matriz){
        Dimension* pos_actual = new Dimension(fila_n, col_n);
        if ((*matriz)[*pos_actual].getColor() != color_pos){
            delete pos_actual;
            if (borrados->esVacia()) {
                i++;
                continue;
            } else {
                break;
            }
        }
        char tipo = (*matriz)[*pos_actual].getTipo();

        if (tipo == MINIBARH || tipo == MINIBARV) {
            int orient_bar = (tipo == MINIBARH) ? FILA : COLUMNA;
            int cant_linea = borrarLinea(*pos_actual, orient_bar, orientacion);
            if (orientacion == orient_bar) i += cant_linea;
            cant += cant_linea;
            delete pos_actual;
        } else {
            *borrados += pos_actual;
            cant++;
            i++;
        }
    }
    return cant;
}

bool Juego::reemplazarOriginal(int len_linea, Dimension& pos, int orientacion){
    if (len_linea <= 4) {
        return false;
    } else if (len_linea == 4){
        if (orientacion == FILA)
            (*matriz)[pos].setTipo(MINIBARH);
        else
            (*matriz)[pos].setTipo(MINIBARV);
    } else {
        (*matriz)[pos].setTipo(ESTRELLA);
        (*matriz)[pos].setColor(ORO);
    }
    return true;
}

bool Juego::procesarLinea(Dimension& pos, int orientacion){
    int largo_linea = buscarLinea(pos, orientacion);
    contarPuntos(largo_linea);
    bool modificado = reemplazarOriginal(largo_linea, pos, orientacion);
    if (! borrarElementos(pos, modificado)) return false;
    return true;
}

int Juego::borrarCelda(Dimension* pos, Celda& celda_no_est, int& cant){
    Celda& actual = (*matriz)[*pos];
    char color_act = actual.getColor();
    char tipo = celda_no_est.getTipo(), color = celda_no_est.getColor();
    int salida = 0;

    if((color != color_act) && (tipo != ESTRELLA))
        return salida;

    if ((tipo == BUTTON) || (tipo == ESTRELLA)){
        *borrados += pos;
        cant++;
        salida = (tipo == ESTRELLA) ? 3 : 0;
    } else if ((tipo == MINIBARH) || (tipo == MINIBARV)){
        int orient_bar = (tipo == MINIBARH) ? FILA : COLUMNA;
        cant += borrarLinea(*pos, orient_bar, FILA);
        salida = (tipo == MINIBARH) ? 1 : 2;
    }
    return salida;
}

int Juego::procesarEstrella(Dimension& una, Dimension& otra){
    Dimension& pos_est= (*matriz)[una].esEstrella() ? una : otra;
    Celda& celda_no_est = (*matriz)[una].esEstrella() ? (*matriz)[otra] : (*matriz)[una];
    Lista<int> col_borradas;
    int salida = 0, cant = 0;

    for(int i=0; i < matriz->getTamanio(); i++){
        for(int j=0; j < matriz->getTamanio(); j++){
            if (col_borradas.pertenece(j)) continue;

            Dimension* pos = new Dimension(i,j);
            salida = borrarCelda(pos, celda_no_est, cant);
            if (salida == 1){
                delete pos;
                break;
            } else if (salida == 2) {
                col_borradas += j;
                delete pos;
            } else if (salida == 3) {
                break;
            }
        }
        if (salida == 3) break;
    }
    return cant;
}

bool Juego::intentarJugada(Dimension& una, Dimension& otra){
    if((*matriz)[una].esEstrella() || (*matriz)[otra].esEstrella())
        procesarEstrella();

    int unaf = una.getFila(), unac = una.getCol();
    int otraf = otra.getFila(), otrac = otra.getCol();
    bool res_una, res_otra;

    if(unaf == otraf && ((unac + otrac) % 2) == 1) {
        res_una = procesarLinea(una, FILA);
        res_otra = procesarLinea(otra, FILA);
    } else if(unac == otrac && ((unaf + otraf) % 2) == 1) {
        res_una = procesarLinea(una, COLUMNA);
        res_otra = procesarLinea(otra, COLUMNA);
    } else {
        return false;
    }

    return (res_una || res_otra);
}

int Juego::realizarJugada(Dimension& una, Dimension& otra){
    int res = matriz->intercambiar(una, otra);
    if (res != 0) return 1;
    if (! intentarJugada(una, otra)){
        matriz->intercambiar(otra, una);
        return 2;
    }
    return 0;
}

void Juego::rellenarCelda(Dimension& pos){
    Celda aux;
    (*matriz)[pos] = aux;
}

void Juego::contarPuntos(int largo_linea){
    if (largo_linea < 3) {
        delete borrados;
        borrados = new Lista<Dimension*>(true);
    } else if (largo_linea == 3) {
        puntaje += PUNTAJE_LINEA_3;
    } else if (largo_linea == 4) {
        puntaje += PUNTAJE_LINEA_4;
    } else {
        puntaje += PUNTAJE_LINEA_5;
    }
}

bool Juego::borrarElementos(Dimension& original, bool modificado){
    if (borrados->esVacia()) return false;

    while (! borrados->esVacia()){
        Dimension* pos = borrados->borrarPrimero();
        int fila = pos->getFila(), col = pos->getCol();

        if (modificado && ((*pos) == original)) {
            continue;
        } else if (fila == 0) {
            rellenarCelda(*pos);
        } else {
            for(int i=fila; i > 0; i--)
                (*matriz)[i][col] = (*matriz)[i-1][col];
            Dimension aux(0, col);
            rellenarCelda(aux);
        }
    }
    return true;
}


bool Juego::hayMovimientos(){
    return false;
}
