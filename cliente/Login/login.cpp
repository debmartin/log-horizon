#include <iostream>

#include "login.h"

#define MAX_CARACTERES 20

//
Login::Login(){
  fondo = new Textura;
  iniciar_sesion = new Boton;
  usuario = new TextBox;
  clave = new TextBoxOculta;
  escritor = new Texto;
  textura_clave = new Textura;
  textura_usuario = new Textura;
  sonido = NULL;
  recibir = NULL;
  enviar = NULL;
  mensaje = new Mensaje;
  conectando = false;
}

//
Login::~Login(){
  delete fondo;
  delete iniciar_sesion;
  delete usuario;
  delete clave;
  delete escritor;
  delete textura_clave;
  delete textura_usuario;
  delete mensaje;
  if (sonido) Mix_FreeChunk(sonido);
}

//
int Login::correr(Ventana *ventana, unsigned int ancho, unsigned  int alto){
  this->cargar_archivos(ventana, ancho, alto);
  
  SDL_Event evento;
  bool corriendo = true;
  FPS frames;
  int tiempo_actual = SDL_GetTicks();
  int delay = 16;
  Mix_PlayChannel(-1, sonido, -1);
  while (corriendo){
    // Eventos
    while (SDL_PollEvent(&evento)){
      corriendo = this->analizar_evento(evento);
    }
    // Dibujado
    ventana->limpiar();
    this->dibujar(ventana);
    // actualizamos los fps
    if (SDL_GetTicks() - tiempo_actual < 1000){
      frames.actualizar();
      delay =  (1000/60.0f) * (frames.ver_fps()/60.0f);
    }
    // Presentar en ventana
    ventana->presentar(delay);
    if (iniciar_sesion->activado()){
      this->enviar_datos();
    }
  }
  return 0;
}

//
int Login::cargar_archivos(Ventana *ventana, unsigned int ancho, unsigned  int alto){
  // FONDO
  fondo->cargar_textura("../../recursos/imagenes/fondo_login.jpg", ventana);
    
  // USUARIO
  std::string fuente_mono = "../../recursos/fuentes/mono.ttf";
  std::string fondo_texto = "../../recursos/imagenes/fondo_texto.png";
  usuario->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  usuario->asignar_fondo(fondo_texto, ventana);
  usuario->alpha_fondo(175);
  SDL_Rect destino;
  destino.x = ancho / 2 - ancho / 8;
  destino.w = ancho * 2 / 8;
  destino.h = alto / 20;
  destino.y = alto / 2 - destino.h * 2;
  usuario->asignar_destino(destino);
  usuario->activar();
  usuario->asignar_color(255,255,255,255);   
  
  destino_usuario.y = destino.y;
  destino_usuario.x = destino.x - destino.w / 2 - 5;
  destino_usuario.h = destino.h;
  destino_usuario.w = destino.w / 2;
  
  // CLAVE
  clave->asignar_fuente(fuente_mono, 50, 3, MAX_CARACTERES);
  clave->asignar_fondo(fondo_texto, ventana);
  clave->alpha_fondo(175);
  destino.y = destino.y + destino.h * 2;
  clave->asignar_destino(destino);
  clave->asignar_color(255,255,255,255);

  destino_clave.y = destino.y;
  destino_clave.x = destino.x - destino.w / 2 - 5;
  destino_clave.h = destino.h;
  destino_clave.w = destino.w / 2;
  
  // BOTON  
  SDL_Rect normal, apretado, sobre;
  normal.x = 0;
  normal.y = 0;
  normal.h = 45;
  normal.w = 102;
  apretado = normal;
  apretado.y = 90;
  sobre = normal;
  sobre.y = 45;
  destino.y = destino.y + destino.h * 2;
  destino.w = destino.w / 2;
  destino.x = destino.x + destino.w / 2;
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino;
  
  iniciar_sesion->asignar_texturas("../../recursos/imagenes/boton.png", estructura, ventana);
  
  Superficie sup;
  Texto texto;
  texto.asignar_fuente("../../recursos/fuentes/orange.ttf", 50);
  texto.asignar_color(220,220,0,255);
  texto.copiar_texto("iniciar sesion", &sup);
  
  destino.x = destino.x + destino.w / 10;
  destino.y = destino.y + destino.h / 10;
  destino.w = destino.w - destino.w / 5;
  destino.h = destino.h - destino.h / 5;
  iniciar_sesion->agregar_texto(&sup, destino, ventana, 1);
  
  // SONIDO
  std::string direccion = "../../recursos/sonidos/login_theme.wav";
  sonido = Mix_LoadWAV(direccion.c_str());
  
  // TEXTO
  escritor->asignar_fuente("../../recursos/fuentes/orange.ttf", 50);
  escritor->asignar_color(0,0,0,255);
  
  escritor->copiar_texto("Usuario:", &sup);
  textura_usuario->cargar_textura(&sup, ventana);
  escritor->copiar_texto("Contraseña:", &sup);
  textura_clave->cargar_textura(&sup, ventana);
  
  // MENSAJE
  sup.cargar("../../recursos/imagenes/sub_ventana.png");
  destino.w = ancho/3;
  destino.h = alto/4;
  destino.x = ancho/2 - destino.w/2;
  destino.y = alto/2 - destino.h/2;
  mensaje->asignar_fondo(&sup, destino, ventana);
  mensaje->establecer_alpha_fondo(205);
  escritor->copiar_texto("Enviando datos...", &sup);
  destino.w = destino.w - destino.w / 5;
  destino.h = destino.h / 4;
  destino.y = destino.y + destino.h * 2 - destino.h / 2;
  destino.x = destino.x + destino.w/8;
  mensaje->asignar_mensaje(&sup, destino, ventana);
  
  return 0;
}

//
int Login::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  usuario->dibujar(ventana);
  clave->dibujar(ventana);
  iniciar_sesion->dibujar(ventana);
  textura_usuario->dibujar(destino_usuario, ventana);
  textura_clave->dibujar(destino_clave, ventana);
  if (conectando) mensaje->dibujar(ventana);
  return 0;
}

//
bool Login::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT) return false;
  usuario->analizar_evento(evento);
  clave->analizar_evento(evento);
  iniciar_sesion->analizar_evento(evento);
  return true;
}

//
void Login::enviar_datos(){
  conectando = true;
}
