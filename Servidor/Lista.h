#ifndef LISTA_H
#define LISTA_H

#include <iostream>

enum errorLista {POSICION_INVALIDA};

template <class> class Lista;
template <class> class ListaIter;

/* Nodo de la lista */
template <class T>
class NodoLista {
    template <class U> friend class Lista;
    template <class W> friend class ListaIter;
    public:
        /* Crea un nodo de lista.
         * Pre: prox es el nodo siguiente al nodo que se crea. Si no hay
         * siguiente, debe ser NULL.
         * Post: Guarda en el nuevo nodo su valor y su nodo siguiente.*/
        NodoLista(const T& valor, NodoLista<T>* prox);
        /* Destruye el nodo.
         * Pre: El nodo fue creado.
         * Post: No se destruyeron ni el dato, ni el siguiente al nodo.*/
        ~NodoLista();

    private:
        T dato;
        NodoLista<T>* siguiente;
};

/* Contenedor lista simplemente enlazada */
template <class U>
class Lista {
    template <class W> friend class ListaIter;
    public:
        /* Crea una lista.
         * Pre: recibe un bool que indica si al destruir la lista se deben
         * destruir los datos que contiene.
         * Post: crea una lista vacia. */
        explicit Lista(bool destruir = false);
        /* Crea una lista por copia.
         * Pre: original fue creada.
         * Post: crea una lista nueva que comparte los datos con la original.*/
        Lista(Lista<U>& original);
        /* Elimina todos los nodos de la lista.
         * Pre: la lista fue creada.
         * Post: si destruirDato era true, se destruyeron todos los datos de la
         * lista. */
        ~Lista();
        /* Asigna a una lista los valores de original.
         * Pre: ambas listas fueron creadas.
         * Post: devuelve una lista cuyo contenido es el mismo que el de la
         * original.*/
        Lista<U>& operator=(Lista<U>& original);
        /* Verifica si la lista está vacia.
        * Pre: la lista fue creada. */
        bool esVacia();
        /* Agrega un elemento a lista.
         * Pre: la lista fue creada.
         * Post: se inserta un nuevo dato aal final de la lista. */
        void insertarUltimo(const U& dato);
        /* Agrega un elemento a lista.
         * Pre: la lista y el iterador fueron creados.
         * Post: se inserta un nuevo dato antes de la posición actual indicada
         * por el iterador.*/
        void insertar(ListaIter<U>* iter, U& dato);
        /* Elimina el primer elemento.
         * Pre: la lista fue creada.
         * Post: se elimina el dato que se encontraba en la posición actual y
         * lo devuelve.
         * Si la lista es vacía, lanza una excepcion de tipo POSICION_INVALIDA.
         */
        U borrarPrimero();
        /* Elimina el elemento de la posición actual.
         * Pre: La lista y el iterador fueron creados.
         * Post: Se elimina el dato que se encontraba en la posición actual
         * indicada por el iterador y se lo devuelve.
         * Si el iterador está al final de la lista, lanza una excepcion de tipo
         * POSICION_INVALIDA.*/
        U borrar(ListaIter<U>* iter);
        /* Devuelve el primer dato.
         * Pre: La lista fue creada.
         * Post: Si la lista es vacía lanza una excepcion de tipo
         * POSICION_INVALIDA. */
        U verPrimero();
        /* Define si se deben destruir los datos junto con la lista.
         * Pre: La lista fue creada.*/
        void setDestruirDato(bool destruir = false) { destruirDato = destruir; }
        /* Devuelve la cantidad de elementos de la lista.
         * Pre: La lista fue creada.*/
        int getCantidad() { return cantidad; }
        /* Devuelve si el dato está en la lista.
         * Pre: La lista fue creada.*/
        bool pertenece(const U& dato);
        /* Agrega al final de la lista el dato.
         * Pre: La lista fue creada.*/
        Lista<U>& operator+=(const U& dato);
        /* Concatena dos listas.
         * Pre: Las listas fueron creadas.
         * Post: Agrega al final de la primera, los elementos de otra.*/
        Lista<U>& operator+=(const Lista<U>& otra);

    private:
        NodoLista<U>* primero;
        NodoLista<U>* ultimo;
        int cantidad;
        bool destruirDato;
};

/* Iterador para recorrer la lista */
template <class W>
class ListaIter {
    template <class U> friend class Lista;
    public:
        /* Construye el iterador de lista.
         * Pre: La lista fue creada.
         * Post: El iterador se encuentra en la primera posicion de la lista.*/
        explicit ListaIter(Lista<W>* lista);
        /* El iterador avanza una posición en la lista.
         * Pre: el iterador fue creado.
         * Post: se avanzó la posición actual en el iterador. Si estaba al
         * final de la lista, lanza una excepcion de tipo POSICION_INVALIDA.*/
        bool avanzar();
        /* Devuelve el elemento actual del iterador.
         * Pre: el iterador fue creado.*/
        W verActual();
        /* Dice si el iterador se encuentra al final de la lista.
         * Pre: el iterador fue creado.*/
        bool alFinal();
        /* Dice si el iterador se encuentra al principio de la lista.
         * Pre: el iterador fue creado.*/
        bool alPrincipio();

    private:
        NodoLista<W>* anterior;
        NodoLista<W>* actual;
};


template <class T>
NodoLista<T>::NodoLista(const T& valor, NodoLista<T>* prox){
    dato = valor;
    siguiente = prox;
}

template <class T>
NodoLista<T>::~NodoLista(){
}

template <class U>
Lista<U>::Lista(bool destruir) :
    primero(NULL), ultimo(NULL), cantidad(0), destruirDato(destruir){
}

template <class U>
Lista<U>::Lista(Lista<U>& original) {
    *this = original;
}

template <class U>
Lista<U>::~Lista(){
    while (! esVacia()) {
        U dato = borrarPrimero();
        if (destruirDato)
            delete dato;
    }
}

template <class U>
Lista<U>& Lista<U>::operator=(Lista<U>& original){
    if (this == &original) return *this;

    this->cantidad = original.cantidad;
    this->destruirDato = original.destruirDato;
    ListaIter<U>* iter = new ListaIter<U>(&original);

    while (! iter->alFinal()) {
        U dato = iter->verActual();
        this->insertarUltimo(dato);
        iter->avanzar();
    }
    delete iter;
    return *this;
}

template <class U>
bool Lista<U>::esVacia(){
    return (cantidad == 0);
}

template <class U>
void Lista<U>::insertarUltimo(const U& dato){
    NodoLista<U>* nuevo = new NodoLista<U>(dato, NULL);

    if (esVacia())
        primero = nuevo;
    if (ultimo)
        ultimo->siguiente = nuevo;
    ultimo = nuevo;
    cantidad++;
}

template <class U>
U Lista<U>::borrarPrimero(){
    if (esVacia()) throw POSICION_INVALIDA;

    NodoLista<U>* aux = primero;
    U valor = aux->dato;

    primero = aux->siguiente;

    if (! aux->siguiente)
        ultimo = NULL;

    delete aux;
    cantidad--;
    return valor;
}

template <class U>
void Lista<U>::insertar(ListaIter<U>* iter, U& dato){
    NodoLista<U>* nuevo = NodoLista<U>(dato, iter->actual);

    if (iter->alPrincipio())
        primero = nuevo;
    else
        iter->anterior->siguiente = nuevo;
    if (iter->alFinal())
        ultimo = nuevo;
    iter->actual = nuevo;
    cantidad++;
}

template <class U>
U Lista<U>::borrar(ListaIter<U>* iter){
    if (iter->alFinal()) throw POSICION_INVALIDA;

    NodoLista<U>* aux = iter->actual;
    U valor = aux->dato;

    if (iter->alPrincipio())
        primero = aux->siguiente;
    else
        iter->anterior->siguiente = aux->siguiente;
    if (! aux->siguiente)
        ultimo = aux->siguiente;

    iter->actual = aux->siguiente;
    delete aux;
    cantidad--;
    return valor;
}

template <class U>
U Lista<U>::verPrimero(){
    return primero->dato;
}

template <class U>
bool Lista<U>::pertenece(const U& dato){
    ListaIter<U>* iter = new ListaIter<U>(this);
    while ((! iter->alFinal()) && (iter->verActual() != dato))
        iter->avanzar();
    if (iter->alFinal())
        return false;
    delete iter;
    return true;
}

template <class U>
Lista<U>& Lista<U>::operator+=(const Lista<U>& otra){
    ListaIter<U>* iter = new ListaIter<U>(&otra);
    while (! iter->alFinal())
        this->insertarUltimo(iter->verActual());
    delete iter;
    return *this;
}

template <class U>
Lista<U>& Lista<U>::operator+=(const U& dato){
    this->insertarUltimo(dato);
    return *this;
}

template <class W>
ListaIter<W>::ListaIter(Lista<W>* lista){
    actual = lista->primero;
    anterior = NULL;
}

template <class W>
bool ListaIter<W>::avanzar(){
    if (this->alFinal()) return false;

    anterior = actual;
    actual = actual->siguiente;
    return true;
}

template <class W>
W ListaIter<W>::verActual(){
    if (this->alFinal()) throw POSICION_INVALIDA;
    return actual->dato;
}

template <class W>
bool ListaIter<W>::alFinal(){
    return (actual == NULL);
}

template <class W>
bool ListaIter<W>::alPrincipio(){
    return (anterior == NULL);
}

#endif // LISTA_H
