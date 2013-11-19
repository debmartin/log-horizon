#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>

#include "receptor_resultados.h"

#define PUNTAJE 3

// Constructor de la clase.
ReceptorResultados::ReceptorResultados(){
  cola = new Lista<resultado_t>;
  recibiendo = false;
  seguir = true;
  socket = NULL;
  mutex = NULL;
}

// Destructor de la clase
ReceptorResultados::~ReceptorResultados(){
  delete cola;
}

//
void ReceptorResultados::agregar_socket(Socket *nuevo_socket){
  socket = nuevo_socket;
}

//
void ReceptorResultados::agregar_mutex(Mutex *nuevo_mutex){
  mutex = nuevo_mutex;
}

//
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
}

//
bool ReceptorResultados::recibiendo_datos(){
  mutex->bloquear();
  return recibiendo;
  mutex->desbloquear();
}

//
bool ReceptorResultados::cola_vacia(){
  return cola->esta_vacia();
}

//
char ReceptorResultados::borrar_siguiente(dato_t &primer_dato, dato_t &segundo_dato){
  mutex->bloquear();
  resultado_t aux = cola->borrar_primero();
  mutex->desbloquear();
  primer_dato = aux.primero;
  segundo_dato = aux.segundo;
  return aux.tipo;
}

//
void ReceptorResultados::finalizar(){
  mutex->bloquear();
  seguir = false;
  mutex->desbloquear();
}
