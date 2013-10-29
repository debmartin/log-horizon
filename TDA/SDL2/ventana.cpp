#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ventana.h"

/* ********************************************************************
 *                            VENTANA
 * ********************************************************************/

// 
Ventana::Ventana(){
  ventana = NULL;
  renderer = NULL;
}

// 
Ventana::~Ventana(){
  if (renderer) SDL_DestroyRenderer(renderer);
  if (ventana) SDL_DestroyWindow(ventana);
}

//
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

//
bool Ventana::dimension_logica(const int ancho, const int alto){
  if (!renderer) return false;
  return SDL_RenderSetLogicalSize(renderer, ancho, alto) == 0;
}

//
void Ventana::titulo(std::string &titulo){
  if (ventana) SDL_SetWindowTitle(ventana, titulo.c_str());
}

//
bool Ventana::limpiar(){
  if (!renderer) return false;
  return SDL_RenderClear(renderer) == 0;
}

//
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

// 
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

//
bool Textura::cargar_textura(Superficie *sup , Ventana *ventana){
  if (!ventana || !sup) return false;
  if (tex){
    SDL_DestroyTexture(tex);
    tex = NULL;
  }
  tex = SDL_CreateTextureFromSurface(ventana->ver_renderer(), sup->ver_surface());
  return tex != NULL;
}
        
// 
bool Textura::dibujar(SDL_Rect *origen, SDL_Rect *destino, Ventana *ventana){
  return SDL_RenderCopy(ventana->ver_renderer(), tex, origen, destino) == 0;
}

// 
bool Textura::dibujar(SDL_Rect *destino, Ventana *ventana){
  return SDL_RenderCopy(ventana->ver_renderer(), tex, NULL, destino) == 0;
}

// 
bool Textura::dibujar(Ventana *ventana){
  return SDL_RenderCopy(ventana->ver_renderer(), tex, NULL, NULL) == 0;
}
