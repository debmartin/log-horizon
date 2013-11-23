#include <iostream>
#include "../../libs/SDL2/SDL.h"

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  
  Ventana ventana;
  ventana.abrir(800,600);
  ventana.dimension_logica(800,600);
  
  TextBox text;                                                        //
  std::string file = "../../recursos/fuentes/mono.ttf";
  if (!text.asignar_fuente(file, 100, 3)) return 1;                       //
  text.asignar_color(255,255,255,255);                                 //
  text.asignar_fondo("../../recursos/imagenes/fondo_texto.png", &ventana);
  text.alpha_fondo(100);
    
  
  SDL_Event evento;
  bool seguir = true;
  SDL_Rect destino;
  destino.x = 0;
  destino.y = 0;
  destino.w = 400;
  destino.h = 200;
  text.asignar_destino(destino);
  while (seguir){
    while (SDL_PollEvent(&evento)){
      if (evento.type == SDL_QUIT)
        seguir = false;
      text.analizar_evento(evento);
    }
    ventana.limpiar();
    text.dibujar(20, &ventana);
    ventana.presentar(20);
  }
  return 0;
}
