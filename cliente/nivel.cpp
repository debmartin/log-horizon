#include <iostream>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/fps.h"
#include "nivel.h"
#include "matriz.h"

// largo inicial del vector animaciones
#define LARGO 15

#define POS_X 180 // posicion en x de la matriz
#define POS_Y 100 // posicion de y en la matriz
#define LARGO_CELDA_X 50
#define LARGO_CELDA_Y 40
#define CANT_ANIMACIONES 1
#define CANT_CELDAS_X 10
#define CANT_CELDAS_Y 8

//
Nivel::Nivel(){
  animaciones = new animacion_t*[LARGO];
  cant_animaciones = 0;
  fondo = NULL;
  seleccion = NULL;
  explosion = NULL;
  tablero = NULL;
  explosion = new Explosion;
}

//
Nivel::~Nivel(){
  if (fondo) delete fondo;
  if (seleccion) delete seleccion;
  if (tablero) delete tablero;
  for (int i = 0; i < cant_animaciones; i++){
    delete animaciones[i]->animacion;
    delete animaciones[i]->textura;
    delete animaciones[i];
  }
  delete[] animaciones;
  delete explosion;
}

//
void Nivel::correr(const std::string &path, Ventana* ventana){
  Nivel::inicializar_datos(path, ventana);
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  while (corriendo){
    while (SDL_PollEvent(&evento)){
      corriendo = Nivel::analizar_evento(evento);
    }
    ventana->limpiar();
    Nivel::dibujar(ventana);
    frames.actualizar();
    Nivel::actualizar_animaciones(frames.ver_fps());
    int delay = (1000.0f/60.0f) * (frames.ver_fps() / 60.0f);
    ventana->presentar(delay);
  }
}

//
void Nivel::inicializar_datos(const std::string &path, Ventana *ventana){
  Superficie *fondo_sup = new Superficie;
  fondo_sup->cargar(path + "fondo1.png");
  fondo_sup->escalar(840,525);
  
  Superficie *fondo_celda = new Superficie;
  fondo_celda->cargar("imagenes/celda_fondo.png");
  
  
  tablero = new Matriz;
  
  // LO SIGUIENTE LO DEBE RECIBIR DEL SERVIDOR
  SDL_Rect dimension;
  dimension.x = CANT_CELDAS_X;
  dimension.y = CANT_CELDAS_Y;
  char **estructura = new char*[dimension.x];
  for (int i = 0; i < dimension.x; i++){
    estructura[i] = new char[dimension.y];
    for (int z = 0; z < dimension.y; z++){
      estructura[i][z] = 1;
    }
  }
  
  estructura[1][5] = 0;
  estructura[1][6] = 0;
  
  estructura[2][0] = 0;
  estructura[2][3] = 0;
  
  estructura[3][5] = 0;
  estructura[3][6] = 0;
  
  estructura[5][0] = 0;
  estructura[5][3] = 0;
  
  // HASTA ACA
  
  SDL_Rect origen;
  origen.x = POS_X;
  origen.y = POS_Y;
  origen.h = LARGO_CELDA_Y;
  origen.w = LARGO_CELDA_X;
  
  tablero->definir_forma(estructura, dimension, origen);
  tablero->dibujar_fondo_celdas(fondo_celda, NULL, fondo_sup);
  
  fondo = new Textura;
  fondo->cargar_textura(fondo_sup, ventana);
  delete fondo_sup;
  delete fondo_celda;

  // ANIMACIONES
  for (int i = 0; i < CANT_ANIMACIONES; i++){
    SpritePos_t sprite;
    SDL_Rect tam;
    tam.x = 0;
    tam.y = 0;
    tam.w = 352;
    tam.h = 40;
    origen;
    origen.x = 0;
    origen.y = 0;
    origen.w = 32;
    origen.h = 40;
    sprite.dimension_total = tam;
    sprite.imagen_inicial = origen;
    sprite.desplazamiento_x = 32;
    sprite.desplazamiento_y = 0;
    
    Superficie *animacion_sup = new Superficie;
    animacion_sup->cargar(path + "estrella.png");
    SDL_Color color;
    animacion_sup->color_pixel(0,0, color);
    animacion_sup->color_clave(color);
    
    Textura* animacion_tex = new Textura;
    animacion_tex->cargar_textura(animacion_sup, ventana);
    delete animacion_sup;
    
    Animacion *animacion = new Animacion;
    animacion->cargar_sprite(sprite);
    animacion_t *animacion_temp = new animacion_t;
    animacion_temp->textura = animacion_tex;
    animacion_temp->animacion = animacion;
    animaciones[i] = animacion_temp;
    cant_animaciones++;
  }
  
  SDL_Rect celda;
  for (int i = 0; i < dimension.x; i++){
    celda.x = i;
    for (int z = 0; z < dimension.y; z++){
      celda.y = z;
      tablero->insertar(animaciones[0]->textura, animaciones[0]->animacion, celda);
    }
  }
  
  // TEXTURA DE SELECCION
  seleccion = new Textura;
  seleccion->cargar_textura("imagenes/celda_seleccion.png", ventana);
  SDL_Rect seleccion_src;
  seleccion_src.x = 0;
  seleccion_src.y = 0;
  seleccion_src.h = 74;
  seleccion_src.w = 69;

  // EXPLOSION
  explosion->cargar_animacion(path, ventana);
  
  sonido = Mix_LoadWAV("sonidos/sound.wav");   // FALTA DEFINIR CLASE SONIDO
}

//
bool Nivel::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT){ 
    return false;
    
  }else if (!tablero->esta_ocupada() && !explosion->explosion_en_curso()){
    if (evento.type == SDL_MOUSEBUTTONDOWN){
      SDL_Rect celda;
      celda.x = (evento.button.x - POS_X) / LARGO_CELDA_X;
      celda.y = (evento.button.y - POS_Y) / LARGO_CELDA_Y;
      if (evento.button.x - POS_X >= 0 && 
      evento.button.y - POS_Y >= 0 && 
      celda.x < CANT_CELDAS_X && 
      celda.y < CANT_CELDAS_Y){
            
        Mix_PlayChannel(-1, sonido, 0); // FALTA DEFINIR CLASE SONIDO
        
        if(evento.button.button == SDL_BUTTON_LEFT){
          SDL_Rect celda_adyacente;
          if (tablero->adyacente_seleccionado(celda, celda_adyacente)){
            tablero->intercambiar(celda, celda_adyacente);
          }else{
            tablero->seleccionar(seleccion, celda);
          }
        }else if(evento.button.button == SDL_BUTTON_RIGHT){
          tablero->quitar_seleccion();
          explosion->explotar(celda, tablero);
        }
      }
    }
  }
  return true;
}

//
void Nivel::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  tablero->dibujar(ventana);
}

void Nivel::actualizar_animaciones(int fps){
  std::cout << fps << std::endl;
  animaciones[0]->animacion->establecer_fps(fps/3.0f);
  explosion->animar(fps/3.0f);
  animaciones[0]->animacion->animar();
  
  if (explosion->finalizada()){
    while (explosion->celdas_vacias()){
      SDL_Rect celda = explosion->borrar_primera();
      tablero->apilar(animaciones[0]->textura, animaciones[0]->animacion, celda);
    }
  }
}

/* ********************************************************************
 *                        EXPLOSION
 * ********************************************************************/

//
Explosion::Explosion(){
  animacion = NULL;
  textura = NULL;
  en_curso = false;
  celdas = new Lista<SDL_Rect>;
}

//
Explosion::~Explosion(){
  delete celdas;
  if (textura) delete textura;
  if (animacion) delete animacion;
}

//
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
  exp_sup->cargar(path + "explosion.png");
  SDL_Color color_exp;
  exp_sup->color_pixel(0,0, color_exp);
  exp_sup->color_clave(color_exp);
  
  textura = new Textura;
  textura->cargar_textura(exp_sup, ventana);
  delete exp_sup;
  
  animacion = new Animacion;
  animacion->cargar_sprite(exp);
}

//
void Explosion::explotar(SDL_Rect &celda, Matriz* tablero){
  en_curso = true;
  tablero->insertar(textura, animacion, celda);
  celdas->insertar_ultimo(celda);
}

//
bool Explosion::explosion_en_curso(){
  return en_curso;
}

//
bool Explosion::finalizada(){
  return animacion->fuera_del_sprite();
}

//
bool Explosion::celdas_vacias(){
  return !celdas->esta_vacia();
}

//
SDL_Rect Explosion::borrar_primera(){
  return celdas->borrar_primero();
}

//
void Explosion::animar(int fps){
  if (animacion->fuera_del_sprite()){
    en_curso = false;
    animacion->animar();
  }
  if (!en_curso) return;
  animacion->establecer_fps(fps);
  animacion->animar();
}
