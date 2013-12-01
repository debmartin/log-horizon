#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "puntaje.h"

//
Puntaje::Puntaje(){
  dibujador_texto = new Texto;
  textura_puntaje = new Textura;
  puntaje = 0;
  destino = NULL;
  ancho_caracter = 0;
  dif_final = 0;
}

//
Puntaje::~Puntaje(){
  delete dibujador_texto;
  delete textura_puntaje;
}

//
bool Puntaje::asignar_fuente(const std::string& path, int font_tam, SDL_Color &color){
  if (!dibujador_texto->asignar_fuente(path, font_tam)) return false;
  dibujador_texto->asignar_color(color);
  return true;
}

//
bool Puntaje::asignar_destino(SDL_Rect &nuevo_destino, Ventana *ventana_dest){
  if (!dibujador_texto->fuente_cargada()) return false;
  if (!destino) destino = new SDL_Rect;
  ventana = ventana_dest;
  *destino = nuevo_destino;
  ancho_caracter = nuevo_destino.w;
  this->actualizar_textura();
  return true;
}

//
void Puntaje::sumar_puntos(unsigned int dif){
  dif_final += dif;
}

//
bool Puntaje::dibujar(){
  if (!destino) return false;
  if (dif_final > 0) this->actualizar_textura();
  return textura_puntaje->dibujar(*destino, ventana);
}

//
void Puntaje::actualizar_textura(){
  std::string puntaje_string;
  if (dif_final > 0){
    puntaje++;
    dif_final--;
    size_t largo_clave = (int)log10((double)puntaje) + 1;
    destino->w = largo_clave * ancho_caracter;
    char *clave = new char[largo_clave + 1];
    snprintf(clave, largo_clave + 1, "%ud", puntaje);
    puntaje_string = clave;
  }else{
    puntaje_string = "0";
  }
  Superficie sup;
  dibujador_texto->copiar_texto(puntaje_string, &sup);
  textura_puntaje->cargar_textura(&sup, ventana);
}
