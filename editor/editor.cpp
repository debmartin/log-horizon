#include <iostream>

#include "editor.h"
#include "creador_nivel.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"

#define RECURSOS "../recursos/"
#define TITULO "Editor de Niveles"

//
Editor::Editor(int ancho, int alto, SocketPrefijo *socket_emisor, SocketPrefijo *socket_receptor){
  emisor = socket_emisor;
  receptor = socket_receptor;
  
  libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  
  ventana = new Ventana;
  ventana->abrir(ancho, alto);
  ventana->dimension_logica(ancho, alto);
  std::string titulo = TITULO;
  ventana->titulo(titulo);
}

//
Editor::~Editor(){
  delete ventana;
}

//
void Editor::correr(){
  bool seguir = this->crear_nivel();
  if (!seguir) return;
/*
  Sala sala;
  std::string path = RECURSOS;
  sala.inicializar(path, ventana, emisor, receptor);
  while (seguir){
    seguir = sala.correr(ventana);
    // path_nivel = sala.ver_nivel();
    if (seguir){
      this->nivel();
    }
  }*/
}

//
bool Editor::crear_nivel(){
  CreadorNivel creador;
  std::string path_recursos = RECURSOS;
  creador.inicializar(path_recursos, ventana, emisor, receptor);
  return creador.correr(ventana);
}
/*
//
bool HDA_Online::editar_nivel(){
  Nivel nivel;
  nivel.inicializar(RECURSOS, ventana, emisor, receptor);
  return nivel.correr(ventana);
}
*/
