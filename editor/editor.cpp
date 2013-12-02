#include <iostream>

#include "editor.h"
#include "creador_nivel.h"
#include "editor_nivel.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"
#include <sys/stat.h>

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
  filas = 0;
  columnas = 0;
}

//
Editor::~Editor(){
  delete ventana;
}

//
void Editor::correr(){
  if (!this->crear_nivel()) return;
  this->editar_nivel();
}

//
bool Editor::crear_nivel(){
  CreadorNivel creador;
  std::string path_recursos = RECURSOS;
  creador.inicializar(path_recursos, ventana, emisor, receptor);
  return creador.correr(ventana, nombre_nivel, columnas, filas);
}

//
bool Editor::editar_nivel(){
  EditorNivel editor_nivel;
  
  std::string path_nivel = "Niveles/" +nombre_nivel;
  mkdir(path_nivel.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  path_nivel += "/";
  std::string path_imagenes = path_nivel + "imagenes";
  mkdir(path_imagenes.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  
  std::string path_recursos = RECURSOS;
  editor_nivel.inicializar(path_recursos, columnas, filas, ventana);
  return editor_nivel.correr(ventana, path_nivel);
}
