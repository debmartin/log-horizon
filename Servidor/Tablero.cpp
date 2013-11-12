#include "Tablero.h"


#define FILA 0
#define COLUMNA 1


Tablero::Tablero(int tam, Lista<Dimension*>* cola_modif)
    : Matriz<Celda>(tam){
    modificados = cola_modif;
    mov_columna = new bool[tam];
}

Tablero::~Tablero(){
    delete mov_columna;
}

int Tablero::intercambiar(Dimension& una, Dimension& otra){
    if (! sonAdyacentes(una, otra)) return -2;

    int res = Matriz::intercambiar(una, otra);
    if (res != 0) return -1;
    int cant = intentarJugada(una, otra);
    if (cant == 0)
        Matriz::intercambiar(una, otra);
    return cant;
}

bool Tablero::intentarJugada(Dimension& una, Dimension& otra){
    if((*this)[una].esEstrella() || (*this)[otra].esEstrella()){
        return buscarConfEspecial(una, otra);
    }
    int cant = 0;
    cant += buscarConfEspecial(una, FILA);
    cant += buscarConfEspecial(una, COLUMNA);
    cant += buscarConfEspecial(otra, FILA);
    cant += buscarConfEspecial(otra, COLUMNA);

    return cant;
}

int Tablero::borrarColumna(int col){
    Dimension dest(tamanio-1, col);
    Dimension origen(-1, col);
    return borrarColumna(dest, origen, true);
}

int Tablero::borrarFila(int fila){
    Dimension inicio(fila, 0);
    Dimension fin(fila, tamanio-1);
    return borrarFila(inicio, fin, true);
}

int Tablero::borrarColumna(Dimension& dest, Dimension& origen, bool borrando){
    int cant = 0;
    int max = dest.getFila();

    for(int i=0; i <= max; i++){
        Dimension actual(dest.getFila() -i, dest.getCol());
        Dimension superior(origen.getFila() -i-1, origen.getCol());

        char tipo = (*this)[actual].getTipo();

        if (tipo == MINIBARH && !borrando){
            Dimension principio(actual.getFila(), 0);
            Dimension final(actual.getFila(), tamanio);
            cant += borrarFila(principio, final, true);
        } else if (tipo == MINIBARV && !borrando) {
            Dimension principio(0, actual.getCol());
            cant += borrarColumna(principio, dest, true);
            max = origen.getFila();
        } else {
            if (superior.esValida())
                (*this)[actual] = (*this)[superior];
            else
                (*this)[actual].rellenar();
            cant++;
            *modificados += new Dimension(actual);
        }
    }
    mov_columna[dest.getCol()] = hayMovimientosCol(dest.getCol());
    return cant;
}

int Tablero::borrarFila(Dimension& inicio, Dimension& fin, bool borrando){
    int max = fin.getCol() - inicio.getCol(), cant = 0;
    for(int i=0; i<max; i++){
        Dimension actual(inicio.getFila(), inicio.getCol()+i);
//        Dimension superior(inicio.getFila()-1, inicio.getCol()+i);

        char tipo = (*this)[actual].getTipo();

        if (tipo == MINIBARH && !borrando){
            cant += borrarColumna(actual, actual, true);
            Dimension principio(actual.getFila(), 0);
            cant += borrarFila(principio, inicio, true);
            max = tamanio;
        } else if (tipo == MINIBARV && !borrando) {
            Dimension origen(-1, actual.getCol());
            Dimension dest(tamanio-1, actual.getCol());
            cant += borrarColumna(dest, origen, true);
        } else {
            cant += borrarColumna(actual, actual, borrando);
        }
    }
    return cant;
}

int Tablero::borrarLinea(Dimension& inicio, Dimension& fin){
    int cant = 0;
    std::cout << inicio.getFila() << "," << inicio.getCol();
    std::cout << fin.getFila() << "," << fin.getCol();
    if (inicio.getFila() == fin.getFila()){
        cant += borrarFila(inicio, fin, false);
    } else {
        cant += borrarColumna(fin, inicio, false);
    }
    return cant;
}

int Tablero::buscarConfEspecial(Dimension& pos, int orientacion){
    int fila = pos.getFila(), col = pos.getCol();
    int tam = tamanio, cant = 0;
    char color_pos = (*this)[pos].getColor();

    int inicio = (orientacion == FILA) ? ((col-2 >= 0) ? col-2 : 0) : ((fila-2  >= 0) ? fila-2 : 0);
    int fin = (orientacion == FILA) ? ((col+2 < tam) ? col+2 : tam-1) : ((fila+2  < tam) ? fila+2 : tam-1);
    int i = inicio;
    int& fila_n = (orientacion == FILA) ? i : fila;
    int& col_n = (orientacion == FILA) ? col : i;
    Dimension inicial(fila_n, col_n), final(fila_n, col_n);

    for(; i<= fin; i++){
        Dimension pos_actual(fila_n, col_n);
        if ((*this)[pos_actual].getColor() != color_pos){
            if (cant >= 3){
                i--;
                Dimension aux(fila_n, col_n);
                final = aux;
                i++;
            }
            continue;
        } else if (i == inicio){
            i++;
            if ((*this)[fila_n][col_n].getColor() != color_pos){
                continue;
                i--;
            }
            i--;
        }
        if (cant == 0)
            inicial = pos_actual;
        cant++;
    }
    if(cant >= 3){
        int res = borrarLinea(inicial, final);
        reemplazarOriginal(cant, pos, orientacion);
        return res;
    }
    return 0;
}

int Tablero::buscarConfEspecial(Dimension& una, Dimension& otra){
    Dimension pos_no_est = ((*this)[una].esEstrella()) ? otra : una;
    Celda& celda_no_est = (*this)[pos_no_est];
    char color_orig = celda_no_est.getColor();
    char tipo_orig = celda_no_est.getTipo();
    int cant = 1;

    if(tipo_orig == MINIBARH || tipo_orig == MINIBARV){
        cant += borrarFila(pos_no_est.getFila());
        cant += borrarColumna(pos_no_est.getCol());
    } else if(tipo_orig == BUTTON) {
        Dimension pos_est = ((*this)[una].esEstrella()) ? una : otra;
        Dimension superior(pos_est.getFila()-1, pos_est.getCol());
        borrarColumna(pos_est, superior, true);
    }
    for(int i=0; i < tamanio; i++){
        if (tipo_orig == ESTRELLA){
            cant += borrarColumna(i);
            continue;
        }
        for(int j=0; j < tamanio; j++){
            Dimension pos_actual(j, i);

            if((*this)[pos_actual].getColor() == color_orig){
                Dimension superior(j-1, i);
                cant += borrarColumna(pos_actual, superior, true);
            }
        }
    }
    return cant;
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

bool Tablero::hayMovimientosCol(int col){
    for(int i=0; i< tamanio-1; i++){
        if ((*this)[i][col].esEstrella())
            return true;

        char color_act = (*this)[i][col].getColor();
        if ((color_act == (*this)[i+2][col].getColor()) == (*this)[i+1][col+1].getColor())
            return true;
        if (col < tamanio-2){
            if ((color_act == (*this)[i][col+2].getColor()) == (*this)[i+1][col+1].getColor())
                return true;
        }
    }
    return false;
}

void Tablero::verMovimientos(){
    for(int i=0; i< tamanio-1; i++){
        mov_columna[i] = hayMovimientosCol(i);
    }
}

bool Tablero::hayMovimientos(){
    for(int i=0; i<tamanio; i++){
        if(mov_columna[i]) return true;
    }
    return false;
}

void Tablero::imprimir(){
    std::cout << "      ";
    for(int i=0; i<tamanio; i++)
        std::cout << i << "     ";
    std::cout << '\n' << '\n';

    for(int i=0; i<tamanio; i++){
        std::cout << i << "     ";
        for(int j=0; j<tamanio; j++){
            Celda salida = (*this)[i][j];
            std::cout << '(' << salida.getColor() << ',' << salida.getTipo() << ')';
                std::cout << "     ";
        }
        std::cout << '\n';
    }
    std::cout << '\n' << '\n';
}
