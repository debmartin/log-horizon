#ifndef MATRIZ_H
#define MATRIZ_H
#include "dimension.h"
#include <iostream>

template <class U>
class Matriz{
    public:
        Matriz(int tam);
        virtual ~Matriz();
        int getTamanio() { return tamanio; }
        U getCelda(int i, int j);
        int intercambiar(Dimension& una, Dimension& otra);
        U* operator[](int i);
        U& operator[](Dimension& pos);
    protected:
        int tamanio;
    private:
        U** tabla;
};




template <class U>
Matriz<U>::Matriz(int tam) : tamanio(tam){
    tabla = new U*[tam];
    for(int i=0; i<tam; i++){
        tabla[i] = new U[tam];
    }
}

template <class U>
Matriz<U>::~Matriz(){
    for(int i=0; i<tamanio; i++){
        delete tabla[i];
    }
    delete tabla;
}

template <class U>
int Matriz<U>::intercambiar(Dimension& una, Dimension& otra){
    int unaf = una.getFila(), unac = una.getCol();
    int otraf = otra.getFila(), otrac = otra.getCol();

    if ((! una.esValida()) || (! otra.esValida()))
        return 1;
    if (unaf >= tamanio || unac >= tamanio || otrac >= tamanio || otraf >= tamanio)
        return 1;

    U aux = tabla[unaf][unac];
    tabla[unaf][unac] = tabla[otraf][otrac];
    tabla[otraf][otrac] = aux;

    return 0;
}

template <class U>
U Matriz<U>::getCelda(int i, int j){
    if ((i >= tamanio) || (j >= tamanio)) throw "Fuera de rango";

    return tabla[i][j];
}

template <class U>
U* Matriz<U>::operator[](int i){
    if (i >= tamanio) throw "Fuera de rango";

    return tabla[i];
}

template <class U>
U& Matriz<U>::operator[](Dimension& pos){
    int fila = pos.getFila(), col = pos.getCol();
    if ((col >= tamanio) || (fila >= tamanio))
        throw "Fuera de rango";

    return tabla[fila][col];
}


#endif // MATRIZ_H
