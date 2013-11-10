#ifndef LISTA_H
#define LISTA_H

#include <iostream>

/* ***************************************************************************
 *                                EXCEPCIONES
 * ***************************************************************************/

// Clase general para las excepciones de la lista
class ListaError: public std::exception{};

// Excepcion si la lista esta vacia y se quiere obtener un dato
class ListaVacia: public ListaError{};


// Clase general para las excepciones del iterador
class IterError: public std::exception{};

// Excepcion si el iterador no tiene una lista que iterar
class ListaInexistente: public IterError{};

// Excepcion si el iter esta al final y se quiere ver el dato actual o borrar
class IterAlFinal: public IterError{};

/* ***************************************************************************
 *                          LISTA CON ITERADOR
 * ***************************************************************************/

template <class T> 
class Lista;

template <class T> 
class Lista_iter;


template <class T>
class Nodo{
  public:
    T dato;
    Nodo<T> *prox;
    
    Nodo(){prox = NULL;}
};

template <class T> 
class Lista{
  friend class Lista_iter<T>;
  public:  
    // Crea una lista vacia.
    Lista(){
      primero = NULL;
      ultimo = NULL;
      largo = 0;
      destruir_dato = NULL;
    }
    
    // Libera la memoria reservada por la lista. Si se ingreso una funcion
    // para destruir los datos con el metodo agregar_destructor, se le
    // aplicara a cada dato de la lista.
    // Post: Se destruyo la lista.
    ~Lista(){
      Nodo<T> *aux = primero;
      
      while (aux){
        if (destruir_dato){
          T dato = aux->dato;
          (*destruir_dato)(dato);
        }
        
        primero = aux->prox;
        delete aux;
        aux = primero;
      }
    }

    
    // Agrega una funcion que se le aplicara a cada dato almacenado en la
    // lista al momento de destruirla
    void agregar_destructor(void (*fdestruir_dato)(T dato)){
      destruir_dato = fdestruir_dato;
    }
    
    // Devuelve verdadero o falso, segun si la lista tiene o no 
    // elementos agregados.
    bool esta_vacia(){
      return largo == 0;
    }

    // Agrega un nuevo elemento al principio de la lista. Devuelve falso 
    // en caso de error.
    // Pre: ~
    // Post: se agrego un nuevo elemento a la lista, dato se encuentra al inicio
    //       de la lista.
    void insertar_primero(T dato){
      Nodo<T>* nodo_nuevo = new Nodo<T>;

      nodo_nuevo->dato = dato;
      
      if (largo == 0){
        primero = nodo_nuevo;
        ultimo = nodo_nuevo;
      }else{
        nodo_nuevo->prox = primero;
        primero = nodo_nuevo;
      }
      largo++;
    }

    // Agrega un nuevo elemento al final de la lista.
    // Pre: ~
    // Post: se agrego un nuevo elemento a la lista, dato se encuentra al final
    //       de la lista.
    void insertar_ultimo(T dato){
      Nodo<T> *nodo_nuevo = new Nodo<T>;

      nodo_nuevo->dato = dato;
      
      if (largo == 0){
        primero = nodo_nuevo;
        ultimo = nodo_nuevo;
      }else{
        ultimo->prox = nodo_nuevo;
        ultimo = nodo_nuevo;
      }
      largo++;
    }

    // Saca el primer elemento de la lista. Si la lista tiene elementos,
    // se quita el primero de la lista, y se devuelve su valor.
    // Pre: La lista no debe estar vacia.
    // Post: se devolvio el valor del primer elemento anterior, la lista
    // contiene un elemento menos.
    T borrar_primero()throw(ListaVacia){
      if (largo == 0) throw (ListaVacia());
      Nodo<T> *aux = primero;
      T dato_aux = aux->dato;
      primero = primero->prox;
      largo--;
      delete aux;
      return dato_aux;
    }
    
    // Obtiene el valor del primer elemento de la lista. Si la lista tiene
    // elementos, se devuelve el valor del primero.
    // Pre: La lista no debe estar vacia.
    // Post: se devolvio el primer elemento de la lista.
    T ver_primero()throw(ListaVacia){
      if (largo == 0) throw ListaVacia();
      return primero->dato;
    }
    
    // Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
    // elementos, se devuelve el valor del ultimo.
    // Pre: La lista no debe estar vacia.
    // Post: se devolvio el ultimo elemento de la lista.
    T ver_ultimo()throw(ListaVacia){
      if (largo == 0) throw ListaVacia();
      return ultimo->dato;
    }

    // Devuelve el valor de la cantidad de elementos de la lista.
    size_t ver_largo(){
      return largo;
    }
    
  private:
    Nodo<T> *primero;
    Nodo<T> *ultimo;
    size_t largo;
    void (*destruir_dato)(T dato);
};

template <class T>
class Lista_iter{
  public:
    // Constructor de la clase. Sin una lista asignada, se debera asignarle una 
    // con el metodo iterar_en_lista.
    Lista_iter(){
      actual = NULL;
      anterior = NULL;
      lista = NULL;
    }
    
    // Asigna una lista al iterador.
    void iterar_en_lista(Lista<T> *lista_a_iterar){
      if (!lista_a_iterar) throw ListaInexistente();
      actual = lista_a_iterar->primero;
      lista = lista_a_iterar;
    }

    // Mueve el iterador al siguiente elemento de la lista. En caso de 
    // encontrarse al final de la lista devuelve false, caso contrario
    // devuelve true.
    // Pre: el iterador debe tener una lista.
    // Post: el iterador avanzo al siguiente elemento.
    bool avanzar()throw(ListaInexistente){
      if (!lista) throw ListaInexistente();
      if (!actual) return false;
      
      anterior = actual;
      actual = actual->prox;
      
      return true;
    }

    // Obtiene el valor del elemento en donde se posiciona el iterador.
    // Pre: el iterador fue creado, debe tener una lista y no debe
    //      encontrarse al final de la lista.
    // Post: se devolvio el valor del elemento donde esta el iterador.
    T ver_actual()throw(ListaVacia, IterAlFinal, ListaInexistente){
      if (!lista) throw ListaInexistente();
      if (lista->esta_vacia()) throw ListaVacia();
      if (Lista_iter::al_final()) throw IterAlFinal();
      return actual->dato;
    }

    // Devuelve verdadero o falso, segun si el iterador se encuentra al
    // final de la lista.
    // Pre: el iterador fue creado.
    // Post: se retorno un valor booleano.
    bool al_final()throw(ListaInexistente){
      if (!lista) throw ListaInexistente();
      return (!actual);
    }


    // Inserta un nuevo elemento a la lista en la posicion del iterador.
    // Pre: El iterador debe estar sobre una lista.
    // Post: se inserto un nuevo elemento en la lista.
    void insertar(T dato)throw(ListaInexistente){
      if (!lista) throw ListaInexistente();
      if (Lista_iter::al_final()){
        lista->insertar_ultimo(dato);
        actual = lista->ultimo;
      }else if (!anterior){
        lista->insertar_primero(dato);
        actual = lista->primero;
      }else{
        Nodo<T> *nodo_nuevo = new Nodo<T>;
        nodo_nuevo->dato = dato;
        nodo_nuevo->prox = actual;
        actual = nodo_nuevo;
        anterior->prox = nodo_nuevo;
        lista->largo++;
      }
    }

    // Borra de la lista el elemento en donde se encuentra el iterador y 
    // devuelve su valor.
    // Pre: El iterador debe estar sobre una lista.
    //      La lista no debe estar vacia.
    //      El iterador no debe estar al final de la lista.
    // Post: Se elmino un elemento de la lista y se devolvio su valor. 
    T borrar()throw(ListaVacia, IterAlFinal, ListaInexistente){
      if (!lista) throw ListaInexistente();
      if (lista->esta_vacia()) throw ListaVacia();
      if (Lista_iter::al_final()) throw IterAlFinal();
      
      if (!anterior){
        actual = lista->primero->prox;
        return lista->borrar_primero();
      }
      Nodo<T>* nodo_aux = actual;
      T dato_aux = actual->dato;
      
      actual = actual->prox;
      delete nodo_aux;
      anterior->prox = actual;
      if (!actual) lista->ultimo = anterior;
      lista->largo--;
      return dato_aux;
    }
  
  private:
    Lista<T> *lista;
    Nodo<T> *actual;
    Nodo<T> *anterior;
};

#endif // LISTA_H
