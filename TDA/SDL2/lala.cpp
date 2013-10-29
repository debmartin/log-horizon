#include <iostream>
#include "ventana.h"
#include "textura.h"

int main(void){
  SDL_Init(SDL_INIT_EVERYTHING);
  Ventana *v = new Ventana;
  v->abrir(800,600);
  Textura *t = new Textura;
  colores_t c;
  c.R = 0;
  c.G = 0;
  c.B = 0;
  t->cargar_textura("fondo1_hda.jpg", v->ver_renderer(),c);
  v->dibujar(t);
  SDL_Event Event;
  bool Running = true;
  while(Running){
     while (SDL_PollEvent(&Event)){
       if (Event.type == SDL_QUIT) Running = false;
     }
    v->limpiar();
    v->dibujar(t);
    v->presentar(10);
  }
  
  delete t;
  delete v;
  SDL_Quit();
  return 0;
}
