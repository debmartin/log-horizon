#include <iostream>
#include "../../libs/SDL2/SDL.h"

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  
  Ventana ventana;
  ventana.abrir(800,600);
  ventana.dimension_logica(800,600);
  
  SDL_Rect destino;
  destino.x = 100;
  destino.y = 100;
  destino.w = 400;
  destino.h = 200;
  
  Boton boton;               
  SDL_Rect normal, apretado, sobre;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
 
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino;
  
  boton.asignar_texturas("../../recursos/imagenes/boton.png", estructura, &ventana);
  
  Superficie sup;
  Texto texto;
  texto.asignar_fuente("../../recursos/fuentes/hda.ttf", 100);
  texto.asignar_color(210,210,50,255);
  texto.copiar_texto("iniciar sesion", &sup);
  
  destino.x = 120;
  destino.y = 105;
  destino.w = 300;
  destino.h = 150;
  boton.agregar_texto(&sup, destino, &ventana, 4);
  
  SDL_Event evento;
  bool seguir = true;

  while (seguir){
    while (SDL_PollEvent(&evento)){
      if (evento.type == SDL_QUIT)
        seguir = false;
      boton.analizar_evento(evento);
    }
    ventana.limpiar();
    
    boton.dibujar(&ventana);
    ventana.presentar(20);
  }
  return 0;
}
