#include <iostream>

#include "mensaje.h"

//
Mensaje::Mensaje(){
  fondo = new Textura;
  mensaje = new Textura;
  escritor = NULL;
}

//
Mensaje::~Mensaje(){
  delete fondo;
  delete mensaje;
}

//
void Mensaje::asignar_escritor(const std::string& fuente, int font_tam){
  escritor = new Texto;
  escritor->asignar_fuente(fuente, font_tam);
}

//
void Mensaje::asignar_fondo(Superficie *sup, SDL_Rect &destino, Ventana *ventana){
  fondo->cargar_textura(sup, ventana);
  destino_fondo = destino;
}

//
void Mensaje::asignar_fondo(const std::string &path, SDL_Rect &destino, Ventana *ventana){
  Superficie sup;
  sup.cargar(path);
  this->asignar_fondo(&sup, destino, ventana);
}
                         
//
void Mensaje::asignar_mensaje(Superficie *sup, SDL_Rect &destino, Ventana *ventana){
  mensaje->cargar_textura(sup, ventana);
  destino_mensaje = destino;
}

//
void Mensaje::asignar_mensaje(const std::string &mensaje, SDL_Rect &destino, Ventana *ventana){
  if (!escritor) return;
  Superficie sup;
  escritor->copiar_texto(mensaje, &sup);
  this->asignar_mensaje(&sup, destino, ventana);
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
