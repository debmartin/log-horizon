#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>

#include "receptor_resultados.h"

#define PUNTAJE 5

// Constructor de la clase.
ReceptorResultados::ReceptorResultados(){
  cola = new Lista<resultado_t>;
  recibiendo = false;
  seguir = true;
  socket = NULL;
  mutex = new Mutex;
}

// Destructor de la clase
ReceptorResultados::~ReceptorResultados(){
  delete cola;
}

// Asigna un socket para poder recibir los datos que seran
// encolados.
// Pre: el socket debe existir
void ReceptorResultados::agregar_socket(Socket *nuevo_socket){
  socket = nuevo_socket;
}

// Funcion a realizar en otro hilo, recibe datos que almacenara
// en la lista, hasta recibir una señal de finalizacion por el metodo
// "finalizar"
void ReceptorResultados::funcion_a_correr(){
  if (!socket || !mutex) return;
  mutex->bloquear();
  while (seguir){
    mutex->desbloquear();
    resultado_t actual;
    socket->recibir(&actual , sizeof(resultado_t));
    
    mutex->bloquear();
    recibiendo = true;
    cola->insertar_ultimo(actual);
    if (actual.tipo == PUNTAJE) recibiendo = false;
  }
  mutex->desbloquear();
}

// devuelve true si se esta recibiendo datos, caso contrario false
bool ReceptorResultados::recibiendo_datos(){
  mutex->bloquear();
  bool respuesta = recibiendo;
  mutex->desbloquear();
  return respuesta;
}

// devuelve true si no hay datos en la cola, caso contrario false
bool ReceptorResultados::cola_vacia(){
  mutex->bloquear();
  bool respuesta =  cola->esta_vacia();
  mutex->desbloquear();
  return respuesta;
}

// Elimina el siguiente dato de la cola y almacena los datos
// en los parametros ingresados y retorna el tipo de mensaje
// desencolado
char ReceptorResultados::borrar_siguiente(dato_t &primer_dato, dato_t &segundo_dato){
  mutex->bloquear();
  resultado_t aux = cola->borrar_primero();
  mutex->desbloquear();
  primer_dato = aux.primero;
  segundo_dato = aux.segundo;
  return aux.tipo;
}

// Devuelve el tipo del siguiente mensaje
char ReceptorResultados::ver_siguiente(){
  mutex->bloquear();
  resultado_t aux = cola->ver_primero();
  mutex->desbloquear();
  return aux.tipo;
}

// Finaliza el hilo
void ReceptorResultados::finalizar(){
  mutex->bloquear();
  seguir = false;
  socket->cerrar_enviar_recibir();
  mutex->desbloquear();
}
