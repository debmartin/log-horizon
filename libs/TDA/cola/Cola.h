#ifndef COLA_H
#define COLA_H

#include <stdbool.h>
#include <stddef.h>
#include <exception>

class ColaFueraDeRango : public std::exception{};

template <class T>
class NodoCola {
    template <class U> friend class Cola;
    public:
        /* Crea un nodo de cola.
         * Pre: prox es el nodo siguiente al nodo que se crea. Si no hay
         * siguiente, debe ser NULL.
         * Post: Guarda en el nuevo nodo su valor y su nodo siguiente.*/
        NodoCola(const T& valor);
        /* Destruye el nodo.
         * Pre: El nodo fue creado.
         * Post: No se destruyeron ni el dato, ni el siguiente al nodo.*/
        ~NodoCola();

    private:
        const T dato;
        NodoCola<T>* nodoSig;
};

template <class U>
class Cola {
    public:
        explicit Cola(bool destruir = false);
        ~Cola();
        bool esVacia();
        void encolar(const U& valor);
        U desencolar();
        U verPrimero();
        Cola<U>& operator+=(const U& dato);
        Cola<U>& operator+=(const Cola<U>& otra);
    private:
        bool destruirDato;
        NodoCola<U>* nodoPrim;
        NodoCola<U>* nodoUlt;
};




template <class T>
NodoCola<T>::NodoCola(const T& valor) :
    dato(valor), nodoSig(NULL) {
}

template <class T>
NodoCola<T>::~NodoCola(){
}

template <class U>
Cola<U>::Cola(bool destruir) :
	destruirDato(destruir), nodoPrim(NULL), nodoUlt(NULL) {
}

template <class U>
Cola<U>::~Cola(){
	while (! esVacia()) {
		U dato = desencolar();
		if (destruirDato)
            delete dato;
	}
}

template <class U>
bool Cola<U>::esVacia(){
	return nodoPrim == NULL;
}

template <class U>
void Cola<U>::encolar(const U& valor){
	NodoCola<U>* nodoNuevo = new NodoCola<U>(valor);

	if (esVacia()) {
		nodoPrim = nodoNuevo;
	} else {
		nodoUlt->nodoSig = nodoNuevo;
	}
	nodoUlt = nodoNuevo;
}

template <class U>
U Cola<U>::verPrimero(){
	if (esVacia()) throw ColaFueraDeRango();
	return nodoPrim->dato;
}

template <class U>
U Cola<U>::desencolar(){
	if (esVacia()) throw ColaFueraDeRango();

	U dato = verPrimero();
	NodoCola<U>* nodoIn = nodoPrim;
	nodoPrim = nodoPrim->nodoSig;
	delete nodoIn;
	return dato;
}

template <class U>
Cola<U>& Cola<U>::operator+=(const U& dato){
    encolar(dato);
    return *this;
}

template <class U>
Cola<U>& Cola<U>::operator+=(const Cola<U>& otra){
    Cola<U> aux;
    while (! otra.esVacia()){
        U& dato = otra.desencolar();
        encolar(dato);
        aux.encolar(dato);
    }
    while (! aux.esVacia())
        otra.encolar(aux.desencolar());

    return *this;
}

#endif // COLA_H
