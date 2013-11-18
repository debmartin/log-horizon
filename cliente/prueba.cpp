#include <iostream>

#include "SDL/SDL.h"
#include "nivel.h"
#include "socket.h"

#define ANCHO 800
#define ALTO 600

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  Ventana *ventana = new Ventana;
  ventana->abrir(ANCHO,ALTO);
  ventana->dimension_logica(ANCHO,ALTO);
  
  Nivel *nivel = new Nivel;
  
  // FALTA PASARLE LOS SOCKETS
  
  nivel->correr("nivel1", ventana, ANCHO, ALTO);
  delete nivel;
  
  return 0;
}
