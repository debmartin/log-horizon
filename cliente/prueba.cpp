#include <iostream>

#include "SDL/SDL.h"
#include "nivel.h"

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  Ventana *ventana = new Ventana;
  ventana->abrir(840,525);
  ventana->dimension_logica(840,525);
  
  Nivel *nivel = new Nivel;
  nivel->correr("imagenes/", ventana);
  delete nivel;
  
  return 0;
}
