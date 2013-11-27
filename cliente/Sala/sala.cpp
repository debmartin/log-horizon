#include <iostream>

#include "sala.h"

#define DELAY 16

//
Sala::Sala(){
  fondo = new Textura;
  crear = new Boton;
  unirse = new Boton;
  escritor = new Texto;
  enviar = NULL;
  recibir = NULL;
}

//
Sala::~Sala(){
  delete fondo;
  delete crear;
  delete unirse;
  delete escritor;
}

//
void Sala::asignar_sockets(Socket* socket_recibir, Socket* socket_enviar){
  recibir = socket_recibir;
  enviar = socket_enviar;
}

//
int Sala::correr(Ventana *ventana, unsigned int ancho, unsigned  int alto){
  this->cargar_archivos(ventana, ancho, alto);
  
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = DELAY;
  while (corriendo){
    // Eventos
    while (SDL_PollEvent(&evento)){
      corriendo = this->analizar_evento(evento);
    }
    // Dibujado
    ventana->limpiar();
    this->dibujar(ventana);
    
    // actualizamos los fps
    this->obtener_delay(frames, tiempo_actual, delay);
    
    // Presentar en ventana
    ventana->presentar(delay);
  }
  return 0;
}

//
void Sala::obtener_delay(FPS &frames, int tiempo_actual, int &delay){
  if (SDL_GetTicks() - tiempo_actual < 1000){
    frames.actualizar();
    delay =  (1000/60.0f) * (frames.ver_fps()/60.0f);
  }
}

//
int Sala::cargar_archivos(Ventana *ventana, unsigned int ancho, unsigned  int alto){
  // FONDO
  fondo->cargar_textura("../../recursos/imagenes/fondo_login.png", ventana);
  
  // TEXTO  
  escritor->asignar_fuente("../../recursos/fuentes/orange.ttf", 50);
  escritor->asignar_color(220,220,0,255);
  
  // BOTON CREAR
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
  destino_boton.x = destino_boton.w / 2;
  
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  crear->asignar_texturas("../../recursos/imagenes/boton.png", estructura, ventana);
  
  Superficie sup;
  escritor->copiar_texto("Crear partida", &sup);
    
  SDL_Rect destino_texto;
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  crear->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // BOTON UNIRSE
  destino_boton.w = ancho / 4;
  destino_boton.h = alto / 10;
  destino_boton.y = alto - destino_boton.h * 2;
  destino_boton.x = destino_boton.w * 5/2;
  
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  unirse->asignar_texturas("../../recursos/imagenes/boton.png", estructura, ventana);
  
  escritor->copiar_texto("Unirse a partida", &sup);
    
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  unirse->agregar_texto(&sup, destino_texto, ventana, 1);
  
  return 0;
}

//
int Sala::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  crear->dibujar(ventana);
  unirse->dibujar(ventana);
  return 0;
}

//
bool Sala::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT) return false;
  crear->analizar_evento(evento);
  unirse->analizar_evento(evento);
  if (crear->activado()){
    // hacer algo
  }
  return true;
}

//
void Sala::enviar_datos(){
  // enviar datos (?)
}
