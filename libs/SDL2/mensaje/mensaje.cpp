#include <iostream>

#include "mensaje.h"

//
Mensaje::Mensaje(){
  fondo = new Textura;
  mensaje = new Textura;
}

//
Mensaje::~Mensaje(){
  delete fondo;
  delete mensaje;
}

//
void Mensaje::asignar_fondo(Superficie *sup, SDL_Rect &destino, Ventana *ventana){
  fondo->cargar_textura(sup, ventana);
  destino_fondo = destino;
}
                         
//
void Mensaje::asignar_mensaje(Superficie *sup, SDL_Rect &destino, Ventana *ventana){
  mensaje->cargar_textura(sup, ventana);
  destino_mensaje = destino;
}

//
void Mensaje::dibujar(Ventana *ventana){
  fondo->dibujar(destino_fondo, ventana);
  mensaje->dibujar(destino_mensaje, ventana);
}

//
void Mensaje::establecer_alpha_fondo(Uint8 alpha){
  fondo->establecer_alpha(alpha);
}
