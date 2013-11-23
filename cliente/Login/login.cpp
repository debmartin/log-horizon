#include <iostream>

#include "login.h"

#define MAX_CARACTERES 20

//
Login::Login(){
  fondo = new Textura;
  iniciar_sesion = new Boton;
  usuario = new TextBox;
  clave = new TextBox;
  sonido = NULL;
}

//
Login::~Login(){
  delete fondo;
  delete iniciar_sesion;
  delete usuario;
  delete clave;
  if (sonido) Mix_FreeChunk(sonido);
}

//
int Login::correr(Ventana *ventana, unsigned int ancho, unsigned  int alto){
  Login::cargar_archivos(ventana, ancho, alto);
  
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = 16;
  Mix_PlayChannel(-1, sonido, -1);
  while (corriendo){
    // Eventos
    while (SDL_PollEvent(&evento)){
      corriendo = Login::analizar_evento(evento);
    }
    // Dibujado
    ventana->limpiar();
    Login::dibujar(ventana);
    // actualizamos los fps
    if (SDL_GetTicks() - tiempo_actual < 1000){
      frames.actualizar();
      delay =  (1000/60.0f) * (frames.ver_fps()/60.0f);
    }
    // Presentar en ventana
    ventana->presentar(delay);
  }
  return 0;
}

//
int Login::cargar_archivos(Ventana *ventana, unsigned int ancho, unsigned  int alto){
  // FONDO
  fondo->cargar_textura("../../recursos/imagenes/fondo_login.png", ventana);
  
  // BOTON
  
  // USUARIO
  std::string fuente_mono = "../../recursos/fuentes/mono.ttf";
  std::string fondo_texto = "../../recursos/imagenes/fondo_texto.png";
  usuario->asignar_fuente(fuente_mono, 50, 3);
  usuario->asignar_fondo(fondo_texto, ventana);
  usuario->alpha_fondo(175);
  SDL_Rect destino;
  destino.x = ancho / 2 - ancho / 8;
  destino.y = alto / 2;
  int a = destino.y;
  std::cout << a << std::endl;
  destino.w = ancho * 2 / 8;
  destino.h = alto / 20;
  usuario->asignar_destino(destino);
  usuario->activar();
  usuario->asignar_color(255,255,255,255);   
  
  // CLAVE
  clave->asignar_fuente(fuente_mono, 50, 3);
  clave->asignar_fondo(fondo_texto, ventana);
  clave->alpha_fondo(175);
  destino.y = destino.y + destino.h * 2;
  a = destino.y;
  std::cout << a << std::endl;
  clave->asignar_destino(destino);
  clave->asignar_color(255,255,255,255);
  clave->ocultar_texto();
  
  // SONIDO
  std::string direccion = "../../recursos/sonidos/login_theme.wav";
  sonido = Mix_LoadWAV(direccion.c_str());
  
  return 0;
}

//
int Login::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  usuario->dibujar(MAX_CARACTERES, ventana);
  clave->dibujar(MAX_CARACTERES, ventana);
  return 0;
}

//
bool Login::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT) return false;
  usuario->analizar_evento(evento);
  clave->analizar_evento(evento);
  return true;
}
