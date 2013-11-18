#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>

#include "cola_resultados.h"

#define PUNTAJE 3

// Constructor de la clase.
ColaResultados::ColaResultados(){
  cola = new Lista<resultado_t>;
  recibiendo_datos = false;
  seguir = true;
  socket = NULL;
  mutex = NULL;
}

// Destructor de la clase
ColaResultados::~ColaResultados(){
  delete cola;
}

//
void ColaResultados::agregar_socket(Socket *nuevo_socket){
  socket = nuevo_socket;
}

//
void ColaResultados::agregar_mutex(Mutex *nuevo_mutex){
  mutex = nuevo_mutex;
}

//
void ColaResultados::funcion_a_correr(){
  if (!socket || !mutex) return;
  mutex->bloquear();
  while (seguir){
    mutex->desbloquear();
    resultado_t actual;
    socket->recibir(&actual , sizeof(resultado_t));
    
    mutex->bloquear();
    recibiendo_datos = true;
    cola->insertar_ultimo(actual);
    if (actual.tipo == PUNTAJE) recibiendo_datos = false;
  }
}

//
bool ColaResultados::recibiendo_datos(){
  mutex->bloquear();
  return recibiendo_datos;
  mutex->desbloquear();
}

//
bool ColaResultados::cola_vacia(){
  return lista->esta_vacia();
}

//
char ColaResultados::borrar_siguiente(dato_t &primer_dato, dato_t &segundo_dato){
  mutex->bloquear();
  resultado_t aux = cola->borrar_primero();
  mutex->desbloquear();
  primer_dato = aux.primero;
  segundo_dato = aux.segundo;
  return aux.tipo;
}

//
void ColaResultados::finalizar(){
  mutex->bloquear();
  seguir = false;
  mutex->desbloquear();
}
