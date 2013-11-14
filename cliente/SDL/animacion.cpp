#include <iostream>
#include <stdlib.h>

#include "animacion.h"
#include "ventana.h"
#include "superficie.h"

/* ********************************************************************
 *                              ANIMACION
 * ********************************************************************/


//
Animacion::Animacion(){
  dimension_total.x = 0;
  dimension_total.y = 0;
  dimension_total.h = 0;
  dimension_total.w = 0;
  imagen_inicial.x = 0;
  imagen_inicial.y = 0;
  imagen_inicial.h = 0;
  imagen_inicial.w = 0;
  imagen_actual.x = 0;
  imagen_actual.y = 0;
  imagen_actual.w = 0;
  imagen_actual.h = 0;
  desp_x = 0;
  desp_y = 0;
  tiempo = 0;
  frame_rate = 0;
}

//
void Animacion::cargar_sprite(SpritePos_t &estructura){
  dimension_total = estructura.dimension_total;
  imagen_inicial = estructura.imagen_inicial;
  imagen_actual = imagen_inicial;
  desp_x = estructura.desplazamiento_x;
  desp_y = estructura.desplazamiento_y;
}

//
bool Animacion::dibujar(Textura* tex, SDL_Rect &destino, Ventana *ventana){
  if (!tex || !ventana) return false;
  return tex->dibujar(imagen_actual, destino, ventana);
}

//
bool Animacion::siguiente(){
  if (Animacion::fuera_del_sprite()) return false;
  imagen_actual.x += desp_x;
  imagen_actual.y += desp_y;
  return true;
}

//
bool Animacion::anterior(){
  if (Animacion::fuera_del_sprite()) return false;
  imagen_actual.x -= desp_x;
  imagen_actual.y -= desp_y;
  return true;
}

//
void Animacion::reiniciar(){
  imagen_actual = imagen_inicial;
}

//
bool Animacion::fuera_del_sprite(){
  return (imagen_actual.x + imagen_actual.w > dimension_total.x + dimension_total.w || 
          imagen_actual.y + imagen_actual.h > dimension_total.y + dimension_total.h ||
          imagen_actual.x < dimension_total.x ||
          imagen_actual.y < dimension_total.y);
}

//
bool Animacion::al_final(){
  return (imagen_actual.x + imagen_actual.w  + desp_x > dimension_total.x + dimension_total.w || 
          imagen_actual.y + imagen_actual.h  + desp_y > dimension_total.y + dimension_total.h);
}

//
bool Animacion::al_inicio(){
  return (imagen_actual.x == imagen_inicial.x && imagen_actual.y == imagen_inicial.y);
}

//
void Animacion::establecer_fps(int fps){
	frame_rate = 1000.0f / fps;
}

//
void Animacion::animar(){
  if (tiempo + frame_rate > SDL_GetTicks()) return;
	Animacion::siguiente();
  tiempo = SDL_GetTicks();
}


/* ********************************************************************
 *                          ANIMACION MOVIL
 * ********************************************************************/

//
AnimacionMovil::AnimacionMovil(){
  pos_actual.x = 0;
  pos_actual.y = 0;
  pos_actual.h = 0;
  pos_actual.w = 0;
  pos_destino.x = 0;
  pos_destino.y = 0;
  pos_destino.h = 0;
  pos_destino.w = 0;
  animacion = NULL;
  textura = NULL;
}

//
void AnimacionMovil::asignar_animacion(Animacion *anim, Textura *tex){
  textura = tex;
  animacion = anim;
}

//
void AnimacionMovil::posicion_actual(SDL_Rect &pos){
  pos_actual = pos;
  pos_destino = pos;
}

//
void AnimacionMovil::mover(SDL_Rect &destino){
  pos_destino = destino;
}

//
SDL_Rect AnimacionMovil::ver_posicion_actual(){
  return pos_actual;
}

//
SDL_Rect AnimacionMovil::ver_posicion_destino(){
  return pos_destino;
}


//
bool AnimacionMovil::movimientos_pendientes(){
  return (pos_actual.x != pos_destino.x || 
          pos_actual.y != pos_destino.y);
}

//
bool AnimacionMovil::dibujar(Ventana *ventana){
  if (!animacion->dibujar(textura, pos_actual, ventana)) return false;
  
  int distancia = pos_destino.x - pos_actual.x;
  if (distancia != 0) pos_actual.x += distancia / abs(distancia);
  
  distancia = pos_destino.y - pos_actual.y;
  if (distancia != 0) pos_actual.y += distancia / abs(distancia);
  
  return true;
}

//
void AnimacionMovil::quitar_textura(){
  textura = NULL;
}
