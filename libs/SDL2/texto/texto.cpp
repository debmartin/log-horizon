#include <iostream>

#include "texto.h"

//
Texto::Texto(){
  color.r = 0;
  color.g = 0;
  color.b = 0;
  color.a = 255;
  fuente = NULL;
}

//
Texto::~Texto(){}

// Setea la fuente a utilizar, abriendo la que recibe como
// parametro
bool Texto::asignar_fuente(const std::string& path, int font_tam){
  if (fuente) TTF_CloseFont(fuente);
  fuente = TTF_OpenFont(path.c_str(), font_tam);
  return fuente != NULL;
}

// Asigna un color al texto
void Texto::asignar_color(Uint8 r, Uint8 g, Uint8 b, Uint8 nivel_alpha){
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = nivel_alpha;
}

// Copia el string 
bool Texto::copiar_texto(const std::string& texto, Superficie *superficie){
  if (!superficie) return false;
  SDL_Surface *temp = TTF_RenderUTF8_Blended(fuente, texto.c_str(), color);
  superficie->asignar_surface(temp);
  return true;
}

//
void Texto::cerrar_fuente(){
  TTF_CloseFont(fuente);
}

//
void Texto::agregar_borde(int espesor){
  TTF_SetFontOutline(fuente, espesor);
}
