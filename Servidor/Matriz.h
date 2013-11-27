#ifndef MATRIZ_H
#define MATRIZ_H
#include "dimension.h"
#include <iostream>
#include <exception>

class MatrizFueraDeRango : public std::exception {};

template <class U>
class Matriz{
    public:
        Matriz(Dimension& tam);
        virtual ~Matriz();
        Dimension getTamanio() { return *tamanio; }
        U getCelda(int i, int j);
        bool intercambiar(Dimension& una, Dimension& otra);
        U* operator[](int i);
        U& operator[](Dimension& pos);
    protected:
        Dimension* tamanio;
    private:
        U** tabla;
};




template <class U>
Matriz<U>::Matriz(Dimension& tam){
    tamanio = new Dimension(tam);
    tabla = new U*[tam.x()];
    for(int i=0; i < tam.x(); i++){
      tabla[i] = new U[tam.y()];
    }
}

template <class U>
Matriz<U>::~Matriz(){
    for(int i=0; i < tamanio->x(); i++){
      delete[] tabla[i];
    }
    delete[] tabla;
    delete tamanio;
}

template <class U>
bool Matriz<U>::intercambiar(Dimension& una, Dimension& otra){
    if ((! una.esValida()) || (! otra.esValida()))
        return false;
    if (una.x() >= tamanio->x() || una.y() >= tamanio->y() || otra.y() >= tamanio->y() || otra.x() >= tamanio->x())
        return false;

    U aux = tabla[una.x()][una.y()];
    tabla[una.x()][una.y()] = tabla[otra.x()][otra.y()];
    tabla[otra.x()][otra.y()] = aux;

    return true;
}

template <class U>
U Matriz<U>::getCelda(int i, int j){
    if ((i >= tamanio->x()) || (j >= tamanio->y())) throw MatrizFueraDeRango();

    return tabla[i][j];
}

template <class U>
U* Matriz<U>::operator[](int i){
    if (i >= tamanio->x()) throw MatrizFueraDeRango();

    return tabla[i];
}

template <class U>
U& Matriz<U>::operator[](Dimension& pos){
    if ((pos.y() >= tamanio->y()) || (pos.x() >= tamanio->x()))
        throw MatrizFueraDeRango();
    return tabla[pos.x()][pos.y()];
}


#endif // MATRIZ_H
