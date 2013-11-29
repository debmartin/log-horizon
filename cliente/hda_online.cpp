#include <iostream>

#include "cliente.h"
#include "Login/login.h"
#include "Sala/sala.h"
#include "Nivel/nivel.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"

#define RECURSOS "../recursos/"
#define TITULO "Hora de Aventura Online"

//
HDA_Online::HDA_Online(int ancho, int alto, Socket *socket_emisor, Socket *socket_receptor){
  emisor = socket_emisor;
  receptor = socket_receptor;
  
  libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  Ventana *ventana = new Ventana;
  ventana->abrir(ancho, alto);
  ventana->dimension_logica(ancho, alto);
  ventana->titulo(TITULO);
}

//
HDA_Online::~HDA_Online(){
  delete ventana;
  delete libreria;
}

//
void HDA_Online::correr(){
  bool seguir = this->login();
  Sala sala;
  sala.inicializar(RECURSOS, ventana, ancho, alto, socket_emisor, socket_receptor);  
  while (seguir){
    seguir = sala.correr(ventana);
    // path_nivel = sala.ver_nivel();
    if (seguir){
      this->nivel();
    }
  }
}

//
bool HDA_Online::login(){
  Login log = Login;
  log.inicializar(RECURSOS, ventana, ancho, alto, socket_emisor, socket_receptor);
  bool seguir = log.correr(ventana);
}

//
bool HDA_Online::nivel(c){
  Nivel nivel;
  nivel.inicializar(RECURSOS, ventana, ancho, alto, socket_emisor, socket_receptor);
  return nivel.correr(ventana);
}
