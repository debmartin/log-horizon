#include <iostream>

#include "seleccion.h"

#define DELAY 16

#define ANTERIOR 2
#define SIGUIENTE 3
#define ACEPTAR 4
#define RETROCEDER 5
#define OK 6
#define ERROR 7
#define FINALIZAR -1

//
Seleccion::Seleccion(){
  fondo = new Textura;
  aceptar = new Boton;
  siguiente = new Boton;
  anterior = new Boton;
  escritor = new Texto;
  retroceder = new Boton;
  iniciar = new Boton;
  partida = new Partida;
  partida_aceptada = false;
  creador = false;
}

//
Seleccion::~Seleccion(){
  delete fondo;
  delete aceptar;
  delete siguiente;
  delete anterior;
  delete escritor;
  delete retroceder;
  delete partida;
  delete iniciar;
}

//
int Seleccion::inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir){
  unsigned int alto = ventana->ver_alto();
  unsigned int ancho = ventana->ver_ancho();
  
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
  
  // BOTON INICIAR
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  iniciar->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  escritor->copiar_texto("Iniciar Partida", &sup);
  
  iniciar->agregar_texto(&sup, destino_texto, ventana, 1);

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

  // PARTIDA
  SDL_Rect destino_partida;
  destino_partida.w = ancho - 100;
  destino_partida.x = 50;
  destino_partida.h = alto/3;
  destino_partida.y = 100; 
  partida->inicializar(recibir, path + "fuentes/orange.ttf", destino_partida, ventana);

  return 0;
}

//
int Seleccion::dibujar(Ventana *ventana){
  siguiente->dibujar(ventana);
  anterior->dibujar(ventana);
  
  retroceder->dibujar(ventana);
  partida->dibujar(ventana);
  if (!partida_aceptada) aceptar->dibujar(ventana);
  else if(creador) iniciar->dibujar(ventana);
  return 0;
}

//
bool Seleccion::analizar_evento(SDL_Event &evento){
  if (!partida_aceptada){
    aceptar->analizar_evento(evento);
    anterior->analizar_evento(evento);
    siguiente->analizar_evento(evento);
    if (anterior->activado()){
      this->enviar_datos(ANTERIOR);
      std::cout << "se envia anterior " << (int) ANTERIOR << std::endl;
    }else if (siguiente->activado()){
      this->enviar_datos(SIGUIENTE);
      std::cout << "se envia siguiente " << (int) SIGUIENTE << std::endl;
    }else if (aceptar->activado()){
      partida_aceptada = true;
      anterior->establecer_alpha(100);
      siguiente->establecer_alpha(100);
      this->enviar_datos(ACEPTAR);
      std::cout << "se envia aceptar " << (int) ACEPTAR << std::endl;
      return false;
    }
  }else{
    iniciar->analizar_evento(evento);
  }
    
  retroceder->analizar_evento(evento);
  if (retroceder->activado()){
    this->enviar_datos(RETROCEDER);
    std::cout << "se envia retroceder " << (int) RETROCEDER << std::endl;
    if (partida_aceptada){
      partida_aceptada = false;
      anterior->establecer_alpha(255);
      siguiente->establecer_alpha(255);
    }else{
      return false;
    }
  }
  return true;
}

//
void Seleccion::partidas_creadas(bool es_creada){
  partida->partidas_creadas(es_creada);
  creador = !es_creada;
}

//
void Seleccion::recibir_datos(){
  partida->recibir_datos();
}

//
void Seleccion::finalizar_recibir_datos(){
  partida->finalizar_recibir();
}

//
void Seleccion::enviar_datos(char eleccion){
  socket_enviar->enviar(&eleccion, sizeof(eleccion));
}
