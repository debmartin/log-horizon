#include <iostream>
#include <stdlib.h>

#include "animacion.h"
#include "../ventana/ventana.h"
#include "../superficie/superficie.h"

/* ********************************************************************
 *                              ANIMACION
 * ********************************************************************/


// constructor de clase
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

// establece la posicion origen de la imagen inicial, el desplazamiento
// para la siguiente imagen y el limite del sprite.
void Animacion::cargar_sprite(SpritePos_t &estructura){
  dimension_total = estructura.dimension_total;
  imagen_inicial = estructura.imagen_inicial;
  imagen_actual = imagen_inicial;
  desp_x = estructura.desplazamiento_x;
  desp_y = estructura.desplazamiento_y;
}

// Dibuja en la ventana ingresada en el rectangulo destino la
// textura, con el origen correspondiente.
// Pre: la textura y la ventana deben existir
// Post: se dibujo la textura en la ventana en la posicion destino 
bool Animacion::dibujar(Textura* tex, SDL_Rect &destino, Ventana *ventana){
  if (!tex || !ventana) return false;
  return tex->dibujar(imagen_actual, destino, ventana);
}

// Ubica la imagen actual en la siguiente posicion
bool Animacion::siguiente(){
  if (Animacion::fuera_del_sprite()) return false;
  imagen_actual.x += desp_x;
  imagen_actual.y += desp_y;
  return true;
}

// Ubica la imagen actual en la posicion anterior
bool Animacion::anterior(){
  if (Animacion::fuera_del_sprite()) return false;
  imagen_actual.x -= desp_x;
  imagen_actual.y -= desp_y;
  return true;
}

// Ubica la imagen actual en la posicion inicial
void Animacion::reiniciar(){
  imagen_actual = imagen_inicial;
}

// Devuelve true en caso de que la animacion se encuentre fuera
// del sprite, caso contrario devolvera false.
// Para volver a ubicarla dentro del sprite se puede usar los
// metodos anterior o reiniciar.
bool Animacion::fuera_del_sprite(){
  return (imagen_actual.x + imagen_actual.w > dimension_total.x + dimension_total.w || 
          imagen_actual.y + imagen_actual.h > dimension_total.y + dimension_total.h ||
          imagen_actual.x < dimension_total.x ||
          imagen_actual.y < dimension_total.y);
}

// devuelve true si la imagen actual se encuentra en la ultima
// imagen, antes de quedar fuera del sprite
bool Animacion::al_final(){
  return (imagen_actual.x + imagen_actual.w  + desp_x > dimension_total.x + dimension_total.w || 
          imagen_actual.y + imagen_actual.h  + desp_y > dimension_total.y + dimension_total.h);
}

// devuelve true si la imagen actual tiene la misma ubicacion
// que la imagen inicial.
bool Animacion::al_inicio(){
  return (imagen_actual.x == imagen_inicial.x && imagen_actual.y == imagen_inicial.y);
}

// establece los fps de la animacion, para poder variar de imagen
// en el ritmo establecido
void Animacion::establecer_fps(int fps){
	frame_rate = 1000.0f / fps;
}

// actualiza la imagen actual, a la siguiente posicion, dependiend
// de los fps ingresados previamente
void Animacion::animar(){
  if (tiempo + frame_rate > SDL_GetTicks()) return;
	Animacion::siguiente();
  tiempo = SDL_GetTicks();
}


/* ********************************************************************
 *                          ANIMACION MOVIL
 * ********************************************************************/

// Constructor de clase
AnimacionMovil::AnimacionMovil(){
  pos_actual.x = 0;
  pos_actual.y = 0;
  pos_actual.h = 0;
  pos_actual.w = 0;
  pos_destino.x = 0;
  pos_destino.y = 0;
  pos_destino.h = 0;
  pos_destino.w = 0;
  distancia = 1;
  animacion = NULL;
  textura = NULL;
}

// Asigna la animacion con su respectiva textura.
void AnimacionMovil::asignar_animacion(Animacion *anim, Textura *tex){
  textura = tex;
  animacion = anim;
}

// Establece la posicion actual en donde será dibujada la animacion
void AnimacionMovil::posicion_actual(SDL_Rect &pos){
  pos_actual = pos;
  pos_destino = pos;
}

// Establece un destino para la animacion, cada vez qeu se la dibuje
// se desplazara una cantidad de pixeles igual a salto hacia el destino
void AnimacionMovil::mover(SDL_Rect &destino, int salto){
  pos_destino = destino;
  distancia = 1;
  if (salto != 0) distancia = salto;
}

// Devuelve la ubicacion actual de la animacion
SDL_Rect AnimacionMovil::ver_posicion_actual(){
  return pos_actual;
}

// Devuelve el destino de la animacion
SDL_Rect AnimacionMovil::ver_posicion_destino(){
  return pos_destino;
}

// Devuelve true si la animacion se esta moviendo
bool AnimacionMovil::movimientos_pendientes(){
  return (pos_actual.x != pos_destino.x || 
          pos_actual.y != pos_destino.y);
}

// Dibuja la animacion en la ventana, en la correspondiente ubicacion
// actual.
bool AnimacionMovil::dibujar(Ventana *ventana){
  if (!animacion->dibujar(textura, pos_actual, ventana)) return false;
  
  int direccion = pos_destino.x - pos_actual.x;
  if (abs(direccion) < abs(distancia)){
    pos_actual.x = pos_destino.x;
  }else if (direccion != 0){
    pos_actual.x += distancia * (direccion / abs(direccion));
  }
  
  direccion = pos_destino.y - pos_actual.y;
  if (abs(direccion) < abs(distancia)){
    pos_actual.y = pos_destino.y;
  }else if (direccion != 0){
    pos_actual.y += distancia * (direccion / abs(direccion));
  }
  
  return true;
}

// Asigna NULL al atributo textura, en este caso al usarse el metodo
// dibujar en la clase, no se dibujara nada.
void AnimacionMovil::quitar_textura(){
  textura = NULL;
}
