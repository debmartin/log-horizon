#include <iostream>

#include "hda_online.h"
#include "Login/login.h"
#include "Sala/sala.h"
#include "Nivel/nivel.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"

#define RECURSOS "../recursos/"
#define TITULO "Hora de Aventura Online"

//
HDA_Online::HDA_Online(int ancho, int alto, SocketPrefijo *socket_emisor, SocketPrefijo *socket_receptor){
  emisor = socket_emisor;
  receptor = socket_receptor;
  
  libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  ventana = new Ventana;
  ventana->abrir(ancho, alto);
  ventana->dimension_logica(ancho, alto);
  std::string titulo = TITULO;
  ventana->titulo(titulo);
}

//
HDA_Online::~HDA_Online(){
  delete ventana;
}

//
void HDA_Online::correr(){
  bool seguir = this->login();
  Sala sala;
  sala.inicializar(RECURSOS, ventana, emisor, receptor);
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
  Login log;
  std::string path_recursos = RECURSOS;
  log.inicializar(path_recursos, ventana, emisor, receptor);
  return log.correr(ventana);
}

//
bool HDA_Online::nivel(){
  Nivel nivel;
  nivel.inicializar(RECURSOS, ventana, emisor, receptor);
  return nivel.correr(ventana);
}
