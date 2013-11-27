#include <iostream>

#include "../../cliente/Sala/sala.h"
#include "../../libs/SDL2/SDL.h"

#define ANCHO 1200
#define ALTO 800

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  Ventana ventana;
  ventana.abrir(ANCHO,ALTO);
  ventana.dimension_logica(ANCHO, ALTO);
  
  Sala sala;
  sala.correr(&ventana, ANCHO, ALTO);
  
  return 0;
}
