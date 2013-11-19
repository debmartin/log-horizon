#include <iostream>
#include "explosion.h"

#define FPS_EXPLOSION 15.0f

// Constructor de clase
Explosion::Explosion(){
  animacion = NULL;
  textura = NULL;
  en_curso = false;
}

// Destructor de clase
Explosion::~Explosion(){
  if (textura) delete textura;
  if (animacion) delete animacion;
}

// Carga la animacion a ser ejecutada
void Explosion::cargar_animacion(const std::string &path, Ventana *ventana){
  SpritePos_t exp;
  SDL_Rect SrcE;
  SDL_Rect DestE;
  SrcE.x = 0;
  SrcE.y = 0;
  SrcE.w = 73;
  SrcE.h = 90;
  DestE.x = 0;
  DestE.y = 0;
  DestE.w = 1022;
  DestE.h = 90;
  exp.dimension_total = DestE;
  exp.imagen_inicial = SrcE;
  exp.desplazamiento_x = 73;
  exp.desplazamiento_y = 0;
  
  Superficie *exp_sup = new Superficie;
  exp_sup->cargar(path + "imagenes/explosion.png");
  SDL_Color color_exp;
  exp_sup->color_pixel(0,0, color_exp);
  exp_sup->color_clave(color_exp);
  
  textura = new Textura;
  textura->cargar_textura(exp_sup, ventana);
  delete exp_sup;
  
  animacion = new Animacion;
  animacion->cargar_sprite(exp);
  animacion->establecer_fps(FPS_EXPLOSION);
}

    // Inserta la animacion asignada anteriormente, en la matriz en
    // la posicion ingresada por parametro
void Explosion::explotar(coordenada_t &celda, Matriz* tablero){
  animacion->reiniciar();
  en_curso = true;
  tablero->insertar(textura, animacion, celda);
}

// Devuelve true si se esta realizando una explosion en el tablero
bool Explosion::explosion_en_curso(){
  return en_curso;
}

// Devuelve true si la explosion finalizo
bool Explosion::finalizada(){
  return animacion->fuera_del_sprite();
}

// Actualiza la animacion de la explosion
void Explosion::animar(){
  if (animacion->fuera_del_sprite()){
    en_curso = false;
  }
  if (!en_curso) return;
  animacion->animar();
}
