#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>

#include "receptor_resultados.h"

#define PUNTAJE 2

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

//
void ReceptorResultados::agregar_socket(Socket *nuevo_socket){
  socket = nuevo_socket;
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
  mutex->desbloquear();
}

//
bool ReceptorResultados::recibiendo_datos(){
  mutex->bloquear();
  bool respuesta = recibiendo;
  mutex->desbloquear();
  return respuesta;
}

//
bool ReceptorResultados::cola_vacia(){
  mutex->bloquear();
  bool respuesta =  cola->esta_vacia();
  mutex->desbloquear();
  return respuesta;
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
char ReceptorResultados::ver_siguiente(){
  mutex->bloquear();
  resultado_t aux = cola->ver_primero();
  mutex->desbloquear();
  return aux.tipo;
}

//
void ReceptorResultados::finalizar(){
  mutex->bloquear();
  seguir = false;
  socket->cerrar_enviar_recibir();
  mutex->desbloquear();
}
