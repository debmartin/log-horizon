#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "superficie.h"

// Constructor de clase
Superficie::Superficie(){
  surface = NULL;
}

// Destructor de clase
Superficie::~Superficie(){
  if (surface) SDL_FreeSurface(surface);
}

// Carga en memoria el contenido del archivo en la superficie
bool Superficie::cargar(const std::string &archivo){
  if (surface) SDL_FreeSurface(surface);
  surface = IMG_Load(archivo.c_str());
  return surface != NULL;
}

// Escala la superficie a la escala ingresada
// Post: Se modifico el tamanio de la superficie
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

// Establece el color clave de la superficie
bool Superficie::color_clave(Uint8 r, Uint8 g, Uint8 b){
  if (!surface) return false;
  Uint32 color = SDL_MapRGB(surface->format, r, g, b);
  return SDL_SetColorKey(surface, SDL_TRUE, color) == 0;
}

// Establece el color clave de la superficie
bool Superficie::color_clave(SDL_Color &color){
  if (!surface) return false;
  Uint32 color_aux = SDL_MapRGB(surface->format, color.r, color.g, color.b);
  return SDL_SetColorKey(surface, SDL_TRUE, color_aux) == 0;
}

// obtiene el color del pixel ingresado y lo almacena en el parametro
// "color"
bool Superficie::color_pixel(int x, int y, SDL_Color &color){
  if (!surface) return false;
  SDL_Color *colores = ((SDL_Color*) surface->pixels);
  color = colores[y * (surface->pitch / sizeof(SDL_Color)) + x];
  return true;
}

