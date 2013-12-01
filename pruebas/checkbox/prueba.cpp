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
  destino.w = 60;
  destino.h = 40;
  
  CheckBox boton;
  SDL_Rect normal, apretado, sobre, apretado_sobre;
  normal.x = 0;
  normal.h = 37;
  normal.w = 42;
  
  normal.y = 74;
  apretado = normal;
  apretado.y = 0;
  sobre = normal;
  sobre.y = 111;
  apretado_sobre = sobre;
  apretado_sobre.y = 37;
 
  estructura_checkbox_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino;
  estructura.resaltado_apretado = apretado_sobre;
  
  boton.asignar_texturas("../../recursos/imagenes/checkbox.png", estructura, &ventana);
  
  Textura fondo;
  fondo.cargar_textura("../../recursos/imagenes/fondo_editor.png", &ventana);
  
  SDL_Event evento;
  bool seguir = true;

  while (seguir){
    while (SDL_PollEvent(&evento)){
      if (evento.type == SDL_QUIT)
        seguir = false;
      boton.analizar_evento(evento);
      if (boton.activado()) std::cout << "esta apretado" << std::endl;
    }
    ventana.limpiar();
    fondo.dibujar(&ventana);
    boton.dibujar(&ventana);
    ventana.presentar(20);
  }
  return 0;
}
