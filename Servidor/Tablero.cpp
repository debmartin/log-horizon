#include "Tablero.h"


Tablero::Tablero(int tam, Lista<Dimension*>* cola_borrados)
    : Matriz<Celda>(tam), jugada_en_curso(NULL){
    borrados = cola_borrados;
    modificados = new Lista<Dimension*>(true);
}

Tablero::~Tablero(){
    delete modificados;
}

int Tablero::intercambiar(Jugada* nueva){
    if(jugada_en_curso) return -1;
    jugada_en_curso = nueva;
    Dimension una(nueva->posicion1()), otra(nueva->posicion2());


    if (! sonAdyacentes(una, otra)) return -1;
    if ((! hayMovimiento(una)) && (! hayMovimiento(otra)))
        return -1;

    Matriz::intercambiar(una, otra);
    return estabilizar(una, otra);
}

int Tablero::estabilizar(Dimension& una, Dimension& otra){
    if((*this)[una].esEstrella() || (*this)[otra].esEstrella()){
        return explosionEstrella(una, otra);
    } else {
        int orientacion = FILA;
        bool ultimo = false;
        Dimension actual(una);

        while(!ultimo){
            Dimension inicial(actual), final(actual);
            int cant = this->buscarConfEspecial(actual, orientacion, inicial, final);

            if(cant >= MIN_LINEA){
                this->borrarLinea(inicial, final);
                this->reemplazarOriginal(cant, actual, orientacion);
            }
            if(orientacion == FILA){
                orientacion = COLUMNA;
            } else if (actual == una) {
                actual = otra;
                orientacion = FILA;
            } else {
                ultimo = true;
            }
        }
    }
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
    int max = dest.x();

    for(int i=0; i <= max; i++){
        Dimension actual(dest.x() -i, dest.y());
        Dimension superior(origen.x() -i-1, origen.y());
        char tipo = (*this)[actual].getTipo();
        Celda salida = (*this)[actual];
            std::cout << '(' << salida.getColor() << ',' << salida.getTipo() << ')';
                std::cout<<actual.x()<<" "<<actual.y()<<"\n";
//    std::cout<<fin.x()<<" "<<su.y()<<"\n
        if (tipo == MINIBARH && !borrando){
            Dimension principio(actual.x(), 0);
            Dimension final(actual.x(), tamanio);
            cant += borrarFila(principio, final, true);
        } else if (tipo == MINIBARV && !borrando) {
            Dimension principio(0, actual.y());
            cant += borrarColumna(principio, dest, true);
            max = origen.x();
        } else {
            if (superior.esValida())
                (*this)[actual] = (*this)[superior];
            else
                (*this)[actual].rellenar();
            cant++;
            *modificados += new Dimension(actual);
        }
    }
    return cant;
}

int Tablero::borrarFila(Dimension& inicio, Dimension& fin, bool borrando){
    int max = fin.y() - inicio.y(), cant = 0;
    for(int i=0; i<=max; i++){
        Dimension actual(inicio.x(), inicio.y()+i);

        char tipo = (*this)[actual].getTipo();

        if (tipo == MINIBARH && !borrando){
            cant += borrarColumna(actual, actual, true);
            Dimension principio(actual.x(), 0);
            cant += borrarFila(principio, inicio, true);
            max = tamanio;
        } else if (tipo == MINIBARV && !borrando) {
            Dimension origen(-1, actual.y());
            Dimension dest(tamanio-1, actual.y());
            cant += borrarColumna(dest, origen, true);
        } else {
            cant += borrarColumna(actual, actual, borrando);
        }
    }
    return cant;
}

int Tablero::borrarLinea(Dimension& inicio, Dimension& fin){
    int cant = 0;
    std::cout<<inicio.x()<<" "<<inicio.y()<<"\n";
    std::cout<<fin.x()<<" "<<fin.y()<<"\n";
    if (inicio.x() == fin.x()){
        cant += borrarFila(inicio, fin, false);
    } else {
        cant += borrarColumna(fin, inicio, false);
    }
    return cant;
}

int Tablero::buscarConfEspecial(Dimension& pos, int orientacion, Dimension& inicial, Dimension& final){
    int fila = pos.x(), col = pos.y();
    int inicio = (orientacion == FILA) ? ((col-2 >= 0) ? col-2 : 0) : ((fila-2  >= 0) ? fila-2 : 0);
    int fin = (orientacion == FILA) ? ((col+2 < tamanio) ? col+2 : tamanio-1) : ((fila+2  < tamanio) ? fila+2 : tamanio-1);
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

int Tablero::explosionEstrella(Dimension& una, Dimension& otra){
    Dimension pos_no_est = ((*this)[una].esEstrella()) ? otra : una;
    Celda& celda_no_est = (*this)[pos_no_est];
    char color_orig = celda_no_est.getColor();
    char tipo_orig = celda_no_est.getTipo();
    int cant = 1;

    if(tipo_orig == MINIBARH || tipo_orig == MINIBARV){
        cant += borrarFila(pos_no_est.x());
        cant += borrarColumna(pos_no_est.y());
    } else if(tipo_orig == BUTTON) {
        Dimension pos_est = ((*this)[una].esEstrella()) ? una : otra;
        borrarColumna(pos_est, pos_est, true);
    }
    for(int i=0; i < tamanio; i++){
        if (tipo_orig == ESTRELLA){
            cant += borrarColumna(i);
            continue;
        }
        for(int j=0; j < tamanio; j++){
            Dimension pos_actual(j, i);

            if((*this)[pos_actual].getColor() == color_orig){
                cant += borrarColumna(pos_actual, pos_actual, true);
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

//provisoria
bool enRango(Dimension& pos, int tam){
    if(!pos.esValida()) return false;
    return ((pos.x()<tam) && (pos.y() <tam));
}

bool Tablero::hayMovimiento(Dimension& pos){
    if ((*this)[pos].esEstrella())
        return true;

    Dimension aux1(pos), aux2(pos);

    for(int i = pos.x()-1; i <= pos.x()+1; i++){
        for(int j = (i==pos.x()) ? pos.y()-1 : pos.y(); (i==pos.x()) ? (j <= pos.y()+1) : (j == pos.y()); j+=2){
            Dimension adyacente(i, j);
            if(!enRango(adyacente, tamanio)) continue;
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
    for(int i=0; i< tamanio; i++){
        for(int j=0; i< tamanio; j++){
            Dimension actual(i,j);
            if (hayMovimiento(actual))
                return true;
        }
    }
    return false;
}

void Tablero::estabilizar(){
    while(! modificados->esVacia()){
        Dimension* pos = modificados->borrarPrimero();
        *borrados += pos;
        for(int i = pos->x()-1; i <= pos->x()+1; i++){
            for(int j = (i==pos->x()) ? pos->y()-1 : pos->y(); (i==pos->x()) ? (j <= pos->y()+1) : (j == pos->y()); j+=2){
                Dimension adyacente(i, j);
                if(!enRango(adyacente, tamanio)) continue;

                int orientacion = FILA;
                bool ultimo = false;

                while(!ultimo){
                    estabilizar(*pos, adyacente);
                    if(orientacion == FILA)
                        orientacion = COLUMNA;
                    else
                        ultimo = true;
                }
            }
        }
    }
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
