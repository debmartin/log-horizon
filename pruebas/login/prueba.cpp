#include <iostream>

#include "../../cliente/Login/login.h"
#include "../../libs/SDL2/SDL.h"

#define ANCHO 800
#define ALTO 600

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  Ventana ventana;
  ventana.abrir(ANCHO,ALTO);
  ventana.dimension_logica(ANCHO, ALTO);
  
  Login log;
  log.correr(&ventana, ANCHO, ALTO);
  
  return 0;
}
