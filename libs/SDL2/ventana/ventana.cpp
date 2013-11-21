#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ventana.h"

/* ********************************************************************
 *                            VENTANA
 * ********************************************************************/

// Constructor de la clase
Ventana::Ventana(){
  ventana = NULL;
  renderer = NULL;
}

// Destructor de la clase
Ventana::~Ventana(){
  if (renderer) SDL_DestroyRenderer(renderer);
  if (ventana) SDL_DestroyWindow(ventana);
}

// Abre una ventana con la dimension ingresada, en pixeles
// Pre: ancho y alto deben ser positivos
// Post: se abrio una ventana de las dimensiones ingresadas.
bool Ventana::abrir(const int ancho, const int alto){
  if (renderer){
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }
  if (ventana){
    SDL_DestroyWindow(ventana);
    ventana = NULL;
  }
  return SDL_CreateWindowAndRenderer(ancho, alto, SDL_WINDOW_RESIZABLE, &ventana, &renderer) == 0;
}

// Establece una dimension logica a la ventana, del alto y ancho
// ingresado.
bool Ventana::dimension_logica(const int ancho, const int alto){
  if (!renderer) return false;
  return SDL_RenderSetLogicalSize(renderer, ancho, alto) == 0;
}

// Establece el titulo de la ventana
void Ventana::titulo(std::string &titulo){
  if (ventana) SDL_SetWindowTitle(ventana, titulo.c_str());
}

// Quita de la ventana todas las texturas dibujadas.
bool Ventana::limpiar(){
  if (!renderer) return false;
  return SDL_RenderClear(renderer) == 0;
}

// Actualiza la ventana con las nevas texturas previamente dibujadas.
bool Ventana::presentar(const unsigned int delay){
  if (!renderer) return false;
  SDL_RenderPresent(renderer);
  SDL_Delay(delay);
  return true;
}

/* ********************************************************************
 *                            TEXTURA
 * ********************************************************************/

// Constructor de la clase
Textura::Textura(){
  tex = NULL;
}

// Destructor de la clase
Textura::~Textura(){
  if (tex) SDL_DestroyTexture(tex);
}

// Abre el archivo ingresado y crea la textura en base a su contenido
// Pre: el archivo debe existir.
// Post: se cargo la textura con el contenido del archivo y se la asocio
//       a la ventana ingresada
bool Textura::cargar_textura(const std::string archivo, Ventana *ventana){
  if (!ventana) return false;
  if (tex){
    SDL_DestroyTexture(tex);
    tex = NULL;
  }
  SDL_Surface *sup = IMG_Load(archivo.c_str());
  if (!sup) return false;
  tex = SDL_CreateTextureFromSurface(ventana->ver_renderer(), sup);
  SDL_FreeSurface(sup);
  return tex != NULL;
}

// Crea la textura en base al contenido de la superficie ingresada
// Pre: la superficie debe existir.
// Post: se cargo la textura con el contenido de la superficie y se la asocio
//       a la ventana ingresada
bool Textura::cargar_textura(Superficie *sup , Ventana *ventana){
  if (!ventana || !sup) return false;
  if (tex){
    SDL_DestroyTexture(tex);
    tex = NULL;
  }
  tex = SDL_CreateTextureFromSurface(ventana->ver_renderer(), sup->ver_surface());
  return tex != NULL;
}
        
// dibuja el rectangulo trazado por el parametro "origen" de la textura,
// en la ubicacion "destino" de la ventana.
// Pre: la ventana debe existir.
// Post: se dibujo la textura en la ventana.
bool Textura::dibujar(SDL_Rect &origen, SDL_Rect &destino, Ventana *ventana){
  return SDL_RenderCopy(ventana->ver_renderer(), tex, &origen, &destino) == 0;
}

// dibuja toda la textura
// en la ubicacion "destino" de la ventana.
// Pre: la ventana debe existir.
// Post: se dibujo la textura en la ventana. 
bool Textura::dibujar(SDL_Rect &destino, Ventana *ventana){
  return SDL_RenderCopy(ventana->ver_renderer(), tex, NULL, &destino) == 0;
}

// Dibuja toda la textura en la ventana, a partir de la ubicacion
// 0,0.
// Pre: la ventana debe existir
// Post: se dibujo la textura en la ventana.
bool Textura::dibujar(Ventana *ventana){
  return SDL_RenderCopy(ventana->ver_renderer(), tex, NULL, NULL) == 0;
}
