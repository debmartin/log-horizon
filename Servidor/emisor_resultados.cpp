#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "emisor_resultados.h"

#define PUNTAJE 3
#define MILISEGUNDOS 10

// Constructor de la clase.
EmisorResultados::EmisorResultados(){
  cola = new Lista<resultado_t>;
  seguir = true;
  socket = NULL;
  mutex = NULL;
}

// Destructor de la clase
EmisorResultados::~EmisorResultados(){
  delete cola;
}

//
void EmisorResultados::agregar_socket(Socket *nuevo_socket){
  socket = nuevo_socket;
}

//
void EmisorResultados::agregar_mutex(Mutex *nuevo_mutex){
  mutex = nuevo_mutex;
}

//
void EmisorResultados::funcion_a_correr(){
  if (!socket || !mutex) return;
  mutex->bloquear();
  while (seguir && !cola->esta_vacia()){
    if (!cola->esta_vacia()){

      resultado_t actual = cola->borrar_primero();

      mutex->desbloquear();
      int a = actual.tipo;

      socket->enviar(&actual , sizeof(resultado_t));
      std::cout << "se enviara resultado del tipo " << a << std::endl;
    }
    mutex->desbloquear();
    usleep(MILISEGUNDOS * 1000);
    mutex->bloquear();
  }
}

//
void EmisorResultados::encolar_dato(char tipo, dato_t &primer_dato, dato_t &segundo_dato){
  mutex->bloquear();
  resultado_t aux;
  aux.tipo = tipo;
  aux.primero = primer_dato;
  aux.segundo = segundo_dato;
  cola->insertar_ultimo(aux);
  mutex->desbloquear();
}

//
void EmisorResultados::finalizar(){
  mutex->bloquear();
  seguir = false;
  mutex->desbloquear();
}
