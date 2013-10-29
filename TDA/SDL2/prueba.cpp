#include <iostream>
#include "ventana.h"
#include "superficie.h"

int main(void){
  SDL_Init(SDL_INIT_EVERYTHING);
  Ventana *v = new Ventana;
  v->abrir(400,100);
  v->dimension_logica(400,100);
  Superficie *s = new Superficie;
  s->cargar("explosion.png");
  SDL_Color c;
  s->color_pixel(0,0,c);
  s->color_clave(c); 
  Textura *t = new Textura;
  t->cargar_textura(s, v);
  SDL_Event Event;
  bool Running = true;
  while(Running){
     while (SDL_PollEvent(&Event)){
       if (Event.type == SDL_QUIT) Running = false;
     }
    v->limpiar();
    t->dibujar(v);
    v->presentar(10);
  }
  
  delete t;
  delete v;
  SDL_Quit();
  return 0;
}

