#include <iostream>

#include "SDL.h"
#include "matriz.h"

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  
  // VENTANA
  Ventana *v = new Ventana;
  v->abrir(840,525);
  v->dimension_logica(840,525);
  
  
  // FONDO CON MATRIZ PEGADA
  Superficie *fondo_sup = new Superficie;
  fondo_sup->cargar("imagenes/fondo.png");
  fondo_sup->escalar(840,525);
  
  Superficie *fondo_celda = new Superficie;
  fondo_celda->cargar("imagenes/celda_fondo.png");
  
  SDL_Rect dimension;
  dimension.x = 10;
  dimension.y = 8;
  
  Matriz *m = new Matriz;
  char **estructura = new char*[dimension.x];
  for (int i = 0; i < dimension.x; i++){
    estructura[i] = new char[dimension.y];
    for (int z = 0; z < dimension.y; z++){
      estructura[i][z] = 1;
    }
  }
  //estructura[5][0] = 0;
  //estructura[5][3] = 0;
  
  SDL_Rect origen;
  origen.x = 180;
  origen.y = 50;
  origen.h = 40;
  origen.w = 50;
  
  m->definir_forma(estructura, dimension, origen);
  
  m->dibujar_fondo_celdas(fondo_celda, NULL, fondo_sup);
  
  Textura *fondo = new Textura;
  fondo->cargar_textura(fondo_sup, v);
  delete fondo_sup;
  delete fondo_celda;

  // ANIMACION
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
  
  Superficie *a = new Superficie;
  a->cargar("imagenes/estrella.png");
  SDL_Color color;
  a->color_pixel(0,0, color);
  a->color_clave(color);
  
  Textura* tex = new Textura;
  tex->cargar_textura(a, v);
  delete a;
  
  Animacion *anim = new Animacion;
  anim->cargar_sprite(sprite);
  
  SDL_Rect celda;
  for (int i = 0; i < dimension.x; i++){
    celda.x = i;
    for (int z = 0; z < dimension.y; z++){
      celda.y = z;
      m->insertar(tex, anim, celda);
    }
  }
  
  // TEXTURA DE SELECCION
  Textura *seleccion = new Textura;
  seleccion->cargar_textura("imagenes/celda_seleccion.png", v);
  SDL_Rect seleccion_src;
  seleccion_src.x = 0;
  seleccion_src.y = 0;
  seleccion_src.h = 74;
  seleccion_src.w = 69;


  SDL_Rect destino;
  destino.x = 50;
  destino.y = 50;
  destino.h = 40;
  destino.w = 50;

  // EXPLOSION

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
  exp_sup->cargar("imagenes/explosion.png");
  SDL_Color color_exp;
  exp_sup->color_pixel(0,0, color_exp);
  exp_sup->color_clave(color_exp);
  
  Textura* exp_tex = new Textura;
  exp_tex->cargar_textura(exp_sup, v);
  delete a;
  
  Animacion *exp_anim = new Animacion;
  exp_anim->cargar_sprite(exp);

  bool en_explosion = false;
  bool explosion_finalizada = false;
  int contador = 5;
  SDL_Event Event;
  bool Running = true;
  SDL_Rect celda2;
  celda2.x = 5;
  celda2.y = 4;
  SDL_Rect celda3;
  celda3.x = 5;
  celda3.y = 5;
  int contador_exp = 3;
  while(Running){
    while (SDL_PollEvent(&Event)){
      if (Event.type == SDL_QUIT) Running = false;
      if (Event.type == SDL_MOUSEBUTTONDOWN){
        if(Event.button.button == SDL_BUTTON_LEFT){
          m->insertar(exp_tex, exp_anim, celda2);
          m->insertar(exp_tex, exp_anim, celda3);
          en_explosion = true;
          
          
        }
        if(Event.button.button == SDL_BUTTON_RIGHT){
          if (m->esta_ocupada()) std::cout << "esta ocupada" << std::endl;
          else std::cout << "NO esta ocupada" << std::endl;
        }
      }
    }
    v->limpiar();
    
    if (explosion_finalizada){
      m->apilar(tex, anim,celda2);
      m->apilar(tex, anim,celda3);
      explosion_finalizada = false;
    }
    
    fondo->dibujar(v);
    m->dibujar(v);
    if (en_explosion){
      contador_exp--;
      if (contador_exp == 0){
        exp_anim->siguiente();
        contador_exp = 3;
      }
      if (exp_anim->fuera_del_sprite()){
        en_explosion = false;
        explosion_finalizada = true;
        exp_anim->reiniciar();
      }
    }
    
    contador--;
    if (contador == 0){
      anim->siguiente();
      contador = 5;
    }
    if (anim->fuera_del_sprite()) anim->reiniciar();
    
    v->presentar(16);
  }  
  
  //delete m;
  delete tex;
  delete anim;
  //delete fondo;
  delete v;
  
  return 0;
}

/*
int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  Ventana *v = new Ventana;
  v->abrir(800,600);
  v->dimension_logica(800,600);
  
  Textura *fondo = new Textura;
  fondo->cargar_textura("imagenes/fondo2.jpg", v);

  SpritePos_t sprite;
  SDL_Rect tam;
  tam.x = 0;
  tam.y = 0;
  tam.w = 352;
  tam.h = 40;
  SDL_Rect origen;
  origen.x = 0;
  origen.y = 0;
  origen.w = 32;
  origen.h = 40;
  sprite.dimension_total = tam;
  sprite.imagen_inicial = origen;
  sprite.desplazamiento_x = 32;
  sprite.desplazamiento_y = 0;
  sprite.repeticiones = -1;
  sprite.contador = 5;
  
  SDL_Rect destino1;
  destino1.x = 100;
  destino1.y = 100;
  destino1.w = 50;
  destino1.h = 50;
  
  SDL_Rect destino2;
  destino2.x = 200;
  destino2.y = 200;
  destino2.w = 50;
  destino2.h = 50;

  Superficie *a = new Superficie;
  a->cargar("imagenes/estrella.png");
  SDL_Color color;
  a->color_pixel(0,0, color);
  a->color_clave(color);
  
  Textura* tex = new Textura;
  tex->cargar_textura(a, v);
  delete a;

  Celda *celda1 = new Celda;
  celda1->asignar_animacion(tex, sprite);
  celda1->asignar_posicion(destino1);

  Celda *celda2 = new Celda;
  celda2->asignar_animacion(tex, sprite);
  celda2->asignar_posicion(destino2);

  SDL_Event Event;
  bool Running = true;
  while(Running){
    while (SDL_PollEvent(&Event)){
      if (Event.type == SDL_QUIT) Running = false;
      if (Event.type == SDL_MOUSEBUTTONDOWN){
        std::cout << "click" << std::endl;
        celda1->intercambiar(celda2);
      }
    }
    v->limpiar();
    
    fondo->dibujar(v);
    celda1->dibujar(v);
    celda2->dibujar(v);
    
    v->presentar(16);
  }  
  
  delete tex;
  delete celda1;
  delete celda2;
  delete fondo;
  delete v;
  
  return 0;
}

*/

/*

int main(void){
  
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  Ventana *v = new Ventana;
  v->abrir(800,600);
  v->dimension_logica(800,600);
  
  Textura *fondo = new Textura;
  fondo->cargar_textura("imagenes/celda_fondo.png", v);
  
  Textura *seleccion = new Textura;
  seleccion->cargar_textura("imagenes/celda_seleccion.png",v);
  
  Textura *fondo_total = new Textura;
  fondo_total->cargar_textura("imagenes/fondo4.png", v);
  
  AnimacionMovil *a = new AnimacionMovil;
  SpritePos_t sprite;
  SDL_Rect tam;
  tam.x = 0;
  tam.y = 0;
  tam.w = 352;
  tam.h = 40;
  SDL_Rect origen;
  origen.x = 0;
  origen.y = 0;
  origen.w = 32;
  origen.h = 40;
  sprite.dimension_total = tam;
  sprite.imagen_inicial = origen;
  sprite.desplazamiento_x = 32;
  sprite.desplazamiento_y = 0;
  sprite.repeticiones = -1;
  a->cargar_sprite("imagenes/estrella.png", sprite, v);
  
  
  bool Running = true;
  SDL_Rect destino1;
  destino1.x = 150;
  destino1.y = 150;
  destino1.w = 50;
  destino1.h = 50;
  
  SDL_Rect destino2;
  destino2.x = 152;
  destino2.y = 152;
  destino2.w = 46;
  destino2.h = 46;
  
  SDL_Rect destino3;
  destino3.x = 154;
  destino3.y = 154;
  destino3.w = 42;
  destino3.h = 42;
  a->posicion_actual(destino3);
  
  int contador = 0;
  SDL_Event Event;
  while(Running){
     while (SDL_PollEvent(&Event)){
       if (Event.type == SDL_QUIT) Running = false;
     }
    v->limpiar();
    
    if (contador == 0) a->mover_horizontal(100,2);
    else if (contador == 100){
      a->mover_vertical(100,2);
      a->mover_horizontal(100,2);
    }
    else if (contador == 200) a->mover_horizontal(100,-2);
    else if (contador == 300){
      a->mover_vertical(100,-2);
      a->mover_horizontal(100,-2);
    }
    else if (contador == 400) contador = -1;
    contador++;
    fondo_total->dibujar(v);
    fondo->dibujar(destino1,v);
    seleccion->dibujar(destino2,v);
    a->dibujar(v);
    
    v->presentar(100);
  }
  
  //delete t;
  delete a;
  delete seleccion;
  delete fondo;
  delete v;
  return 0;
}
*/
