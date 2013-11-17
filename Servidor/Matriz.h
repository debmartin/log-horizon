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
        bool intercambiar(Dimension& una, Dimension& otra);
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
bool Matriz<U>::intercambiar(Dimension& una, Dimension& otra){
    if ((! una.esValida()) || (! otra.esValida()))
        return false;
    if (una.x() >= tamanio || una.y() >= tamanio || otra.y() >= tamanio || otra.x() >= tamanio)
        return false;

    U aux = tabla[una.x()][una.y()];
    tabla[una.x()][una.y()] = tabla[otra.x()][otra.y()];
    tabla[otra.x()][otra.y()] = aux;

    return true;
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
    if ((pos.y() >= tamanio) || (pos.x() >= tamanio))
        throw "Fuera de rango";

    return tabla[pos.x()][pos.y()];
}


#endif // MATRIZ_H
