#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "superficie.h"

//
Superficie::Superficie(){
  surface = NULL;
}

//
Superficie::~Superficie(){
  if (surface) SDL_FreeSurface(surface);
}

//
bool Superficie::cargar(const std::string &archivo){
  if (surface) SDL_FreeSurface(surface);
  surface = IMG_Load(archivo.c_str());
  return surface != NULL;
}

//
bool Superficie::escalar(int ancho, int alto){
  if (!surface) return false;
  SDL_Surface *temp = SDL_CreateRGBSurface(surface->flags, ancho, alto, surface->format->BitsPerPixel,
        surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);
  if (!temp) return false;
  SDL_BlitScaled(surface, NULL, temp, NULL);
  
  SDL_FreeSurface(surface);
  surface = temp;
  return true;
}

//
bool Superficie::color_clave(Uint8 r, Uint8 g, Uint8 b){
  if (!surface) return false;
  Uint32 color = SDL_MapRGB(surface->format, r, g, b);
  return SDL_SetColorKey(surface, SDL_TRUE, color) == 0;
}

//
bool Superficie::color_clave(SDL_Color &color){
  if (!surface) return false;
  Uint32 color_aux = SDL_MapRGB(surface->format, color.r, color.g, color.b);
  return SDL_SetColorKey(surface, SDL_TRUE, color_aux) == 0;
}

//
bool Superficie::color_pixel(int x, int y, SDL_Color &color){
  if (!surface) return false;
  SDL_Color *colores = ((SDL_Color*) surface->pixels);
  color = colores[y * (surface->pitch / sizeof(SDL_Color)) + x];
  return true;
}
