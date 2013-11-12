#ifndef HASH_H
#define HASH_H

#include <string>
#include <cstring>
#include "Lista.h"

#define TAM_MIN 10
#define MAX_LARGO_LISTA 4

enum errorHash {CLAVE_INEXISTENTE};

template <class> class Hash;

/* Nodo del hash */
template <class K>
class NodoHash {
    template <class L> friend class Hash;
    public:
        /* Construye un nodo.
         * Pre: str_clave fue creado. El valor de destruir determina si los
         * datos se destruiran junto con el nodo.
         * Post: Guarda una clave una copia de str_clave y agrega el dato.*/
        NodoHash(std::string& str_clave, K& dato, bool destruir = false);
        /* Destruye el nodo.
         * Pre: El nodo fue creado.
         * Post: Destruye la lista que contiene los datos del nodo. Si al
         * crearlo destruir era true, se destruyen tambien los datos.*/
        ~NodoHash();
        /* Guarda en str_clave el una copia de la clave del nodo.
         * Pre: El nodo fue creado. str_clave fue creada.
         * Post: Si str_clave no era vacía, se borran los datos anteriores.*/
        void getClave(std::string& str_clave);
        /* Devuelve una referencia a la lista de datos guardada en el nodo.
         * Pre: El nodo fue creado.*/
        Lista<K>& getValor();
        /* Agrega al nodo un nuevo dato.
         * Pre: El nodo fue creado.
         * Post: Se agrego el dato, sin borrar los anteriores.*/
        void guardar(K& dato);

    private:
        const std::string clave;
        Lista<K>* valor;
        bool destruirDato;
};

/* Contenedor Hash */
template <class L>
class Hash {
    public:
        /* Construye un hash.
         * Pre: destruir debe ser true si se desea que los datos se destruyan
         * junto con el hash.
         * Post Devuelve un hash vacío.*/
        explicit Hash(bool destruir = false);
        /* Destruye el hash.
         * Pre: El hash fue creado.
         * Post: Se destruyo el hash. Si se indicó al crearlo que se debían
         * destruir los datos que contiene, se destruyen.*/
        ~Hash();
        /* Devuelve la cantidad de elementos guardados en el hash.
         * Pre: El hash fue creado.*/
        size_t getCantidad() { return cantidad; }
        /* Agrega un nuevo dato al hash.
         * Pre: El hash y str_clave fueron creados.
         * Post: Se agrego dato al hash. Si str_clave ya existía en el hash, no
         * se superpone su valor anterior, sino que se agrega el nuevo dato.*/
        void guardar(std::string& str_clave, L& dato);
        /* Borra los datos correspondientes a str_clave del hash.
         * Pre: El hash, valor y str_clave fueron creados.
         * Post: Copia a valor todos los datos que pertenecían a str_clave en
         * el hash. Borra del hash la clave y sus datos.
         * Si str_clave no existe en el hash, lanza una excepcion de tipo
         * CLAVE_INEXISTENTE.*/
        void borrar(std::string& str_clave, Lista<L>& valor);
        /* Devuelve una referencia a los datos que se hallan en el hash,
         * correspondientes a str_clave.
         * Pre: El hash y str_clave fueron creados.
         * Post: Si str_clave no existe en el
         * hash, lanza una excepcion de tipo CLAVE_INEXISTENTE.*/
        Lista<L>& obtener(std::string& clave);
        /* Devuelve si la str_clave está en el hash o no.
         * Pre: El hash y str_clave fueron creados.*/
        bool pertenece(std::string& str_clave);

    private:
        bool destruirDato;
        Lista<NodoHash<L>*>* tabla;
        size_t tamanio;
        size_t cantidad;
        bool redimensionando;
        /* Dada una clave, devuelve un valor único relacionado con ella.
         * Pre: La clave y el hash fueron cread.
         * Post: Para la misma clave el valor devuelto es siempre el mismo.*/
        size_t funHash(const std::string clave);
        /* Busca el nodo del hash que corresponde a str_clave.
         * Pre: El hash y str_clave fueron creados.
         * Post: Si borrar era true, elimina el nodo del hash. Devuelve el nodo
         */
        NodoHash<L>* obtenerNodo(std::string& str_clave, bool borrar);
        /* Cambia el tamanio de la tabla por nuevo_tam.
         * Pre: El hash fue creado.
         * Post: Todos los elementos fueron reubicados dentro de la tabla segun
         * la posicion dada por funHash.*/
        void redimensionar(size_t nuevo_tam);
};



template <class K>
NodoHash<K>::NodoHash(std::string& str_clave, K& dato, bool destruir) :
    clave(str_clave), destruirDato(destruir) {
    valor = new Lista<K>(destruir);
    guardar(dato);
}

template <class K>
NodoHash<K>::~NodoHash(){
    delete valor;
}

template <class K>
void NodoHash<K>::getClave(std::string& str_clave){
    str_clave.assign(clave);
}

template <class K>
Lista<K>& NodoHash<K>::getValor(){
    return *valor;
}

template <class K>
void NodoHash<K>::guardar(K& dato){
    valor->insertarUltimo(dato);
}

template <class L>
size_t Hash<L>::funHash(const std::string clave){
    int hash = 0, largo = clave.length();

    for (int i=0; i < largo; i++){
        hash = clave[i] + (hash << 6) + (hash << 8) - hash;
    }
    return hash % tamanio;
}

template <class L>
Hash<L>::Hash(bool destruir) :
    destruirDato(destruir), redimensionando(false){
    tabla = new Lista<NodoHash<L>*>[TAM_MIN];
    for (int i=0; i<TAM_MIN; i++)
        tabla[i].setDestruirDato(true);
    tamanio = TAM_MIN;
    cantidad = 0;
}

template <class L>
Hash<L>::~Hash(){
    delete[] tabla;
}

template <class L>
NodoHash<L>* Hash<L>::obtenerNodo(std::string& str_clave, bool borrar){
    size_t pos = funHash(str_clave.c_str());

    ListaIter<NodoHash<L>*>* iter = new ListaIter<NodoHash<L>*>(&(tabla[pos]));
    while (! iter->alFinal()){
        NodoHash<L>* aux = iter->verActual();
        std::string aux_clave;
        aux->getClave(aux_clave);
        if (str_clave == aux_clave) {
            if (borrar) tabla[pos].borrar(iter);
            delete iter;
            return aux;
        }
        iter->avanzar();
    }
    delete iter;
    throw CLAVE_INEXISTENTE;
}

template <class L>
void Hash<L>::guardar(std::string& clave, L& dato){
    NodoHash<L>* nodo;
    try {
        nodo = obtenerNodo(clave, false);
        nodo->guardar(dato);
    } catch(errorHash CLAVE_INEXISTENTE) {
        nodo = new NodoHash<L>(clave, dato, destruirDato);
        size_t i = funHash(clave.c_str());
        tabla[i].insertarUltimo(nodo);
        cantidad++;
    }

    if ((tamanio * MAX_LARGO_LISTA <= cantidad) && !redimensionando)
        this->redimensionar(tamanio * MAX_LARGO_LISTA * 2);
}

template <class L>
void Hash<L>::borrar(std::string& clave, Lista<L>& valor){
    NodoHash<L>* aux = obtenerNodo(clave, true);
    valor = aux->getValor();
    cantidad--;
    delete aux;
}

template <class L>
Lista<L>& Hash<L>::obtener(std::string& clave){
    NodoHash<L>* aux = obtenerNodo(clave, false);
    return aux->getValor();
}

template <class L>
bool Hash<L>::pertenece(std::string& clave){
    try {
        obtenerNodo(clave, false);
    } catch(errorHash CLAVE_INEXISTENTE) {
        return false;
    }
    return true;
}

template <class L>
void Hash<L>::redimensionar(size_t nuevo_tam){
    redimensionando = true;

    Lista<NodoHash<L>*>* aux = new Lista<NodoHash<L>*>[nuevo_tam];

    for (size_t i=0; i<tamanio; i++){
        ListaIter<NodoHash<L>*>* iter = new ListaIter<NodoHash<L>*>(&tabla[i]);

        while (! iter->alFinal()){
            NodoHash<L>* nodo = iter->verActual();
            size_t pos = funHash(nodo->clave);
            aux[pos].insertarUltimo(nodo);
        }
        delete iter;
    }
    delete tabla;
    tabla = aux;
    tamanio = nuevo_tam;
    redimensionando = false;
}


#endif // HASH_H
