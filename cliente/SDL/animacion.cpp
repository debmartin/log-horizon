#include <iostream>

#include "animacion.h"
#include "ventana.h"
#include "superficie.h"

#define X 0
#define Y 0

//
Animacion::Animacion(){
  tex = NULL;
}

//
Animacion::~Animacion(){
  if (tex) delete tex;
}

//
bool Animacion::cargar_sprite(const std::string &archivo, SpritePos_t &estructura, Ventana* ventana){
  if (tex) delete tex;
  // abrimos el archivo y establecemos color clave
  Superficie *sup = new Superficie;
  sup->cargar(archivo);             // devuelve bool
  SDL_Color color;
  sup->color_pixel(X,Y, color);     // devuelve bool
  sup->color_clave(color);
  tex = new Textura;
  tex->cargar_textura(sup, ventana);
  delete sup;
  return Animacion::cargar_sprite(sup, estructura, ventana);
}

//
bool Animacion::cargar_sprite(Superficie *sup, SpritePos_t &estructura, Ventana *ventana){
  if (!ventana || !sup) return false;
  
  dimension_total = estructura.dimension_total;
  imagen_inicial = estructura.imagen_inicial;
  imagen_actual = imagen_inicial;
  desp_x = estructura.desplazamiento_x;
  desp_y = estructura.desplazamiento_y;
  int x = desp_x;
  int y = desp_y;
  std::cout << "desp_x =" << x << " desp_y =" << y << std::endl;
  return true;
}

//
bool Animacion::dibujar(SDL_Rect &destino, Ventana *ventana){
  if (!tex || !ventana) return false;
  tex->dibujar(imagen_actual, destino, ventana);    //devuelve bool
  imagen_actual.x += desp_x;
  imagen_actual.y += desp_y;
  if (imagen_actual.x >= dimension_total.w || 
      imagen_actual.y >= dimension_total.h)
    imagen_actual = imagen_inicial;
  return true;
}  
