#include <iostream>

#include "seleccion.h"

#define DELAY 16

//
Seleccion::Seleccion(){
  fondo = new Textura;
  aceptar = new Boton;
  siguiente = new Boton;
  anterior = new Boton;
  escritor = new Texto;
  retroceder = new Boton;
}

//
Seleccion::~Seleccion(){
  delete fondo;
  delete aceptar;
  delete siguiente;
  delete anterior;
  delete escritor;
  delete retroceder;
}

//
int Seleccion::inicializar(const std::string &path, Ventana *ventana, unsigned int ancho, unsigned int alto, Socket* enviar, Socket* recibir){
  socket_recibir = recibir;
  socket_enviar = enviar;
    
  // TEXTO  
  escritor->asignar_fuente(path + "fuentes/orange.ttf", 50);
  escritor->asignar_color(220,220,0,255);
  
  // BOTON ANTERIOR
  SDL_Rect normal, apretado, sobre;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
  
  SDL_Rect destino_boton;
  destino_boton.w = ancho / 4;
  destino_boton.h = alto / 10;
  
  destino_boton.y = alto - destino_boton.h * 2;
  destino_boton.x = ancho / 4 - destino_boton.w / 2;
  
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  anterior->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  Superficie sup;
  escritor->copiar_texto("< Anterior", &sup);
    
  SDL_Rect destino_texto;
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  anterior->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // BOTON SIGUIENTE
  destino_boton.x = ancho * 3/4 - destino_boton.w / 2;
  
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  siguiente->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  escritor->copiar_texto("Siguiente >", &sup);
    
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  siguiente->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // BOTON ACEPTAR
  destino_boton.y = alto/2;
  destino_boton.x = ancho/2 - destino_boton.w / 2;
  
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  aceptar->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  escritor->copiar_texto("Aceptar", &sup);
    
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  aceptar->agregar_texto(&sup, destino_texto, ventana, 1);

  // BOTON RETROCEDER

  destino_boton.y = 10;
  destino_boton.x = 10;
  
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  retroceder->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  escritor->copiar_texto("Retroceder", &sup);
    
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  retroceder->agregar_texto(&sup, destino_texto, ventana, 1);


  return 0;
}

//
int Seleccion::dibujar(Ventana *ventana){
  siguiente->dibujar(ventana);
  anterior->dibujar(ventana);
  aceptar->dibujar(ventana);
  retroceder->dibujar(ventana);
  return 0;
}

//
bool Seleccion::analizar_evento(SDL_Event &evento){
  aceptar->analizar_evento(evento);
  anterior->analizar_evento(evento);
  siguiente->analizar_evento(evento);
  retroceder->analizar_evento(evento);
  if (retroceder->activado()) return false;
  return true;
}
