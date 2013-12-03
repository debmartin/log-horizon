#include <iostream>

#include "sala.h"

#define DELAY 16

#define CREAR 0
#define UNIRSE 1
#define FINALIZAR -1

//
Sala::Sala(){
  fondo = new Textura;
  crear = new Boton;
  unirse = new Boton;
  seleccion = new Seleccion;
  escritor = new Texto;
  socket_enviar = NULL;
  seleccionando = false;
}

//
Sala::~Sala(){
  delete fondo;
  delete crear;
  delete unirse;
  delete seleccion;
  delete escritor;
}

//
int Sala::inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir){
  unsigned int ancho = ventana->ver_ancho();
  unsigned int alto = ventana->ver_alto();
  
  // SELECCION
  seleccion->inicializar(path, ventana, enviar, recibir);
  seleccion->recibir_datos();
  
  // SOCKETS
  socket_enviar = enviar;
  socket_recibir = recibir;
  // FONDO
  fondo->cargar_textura(path + "imagenes/fondo_sala.png", ventana);
  
  // TEXTO  
  escritor->asignar_fuente(path + "fuentes/orange.ttf", 50);
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
  destino_boton.x = ancho / 4 - destino_boton.w / 2;
  
  estructura_boton_t estructura;
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  crear->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  Superficie sup;
  escritor->copiar_texto("Crear", &sup);
    
  SDL_Rect destino_texto;
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  destino_texto.w = destino_boton.w - destino_boton.w / 5;
  destino_texto.h = destino_boton.h - destino_boton.h / 5;
  crear->agregar_texto(&sup, destino_texto, ventana, 1);
  
  // BOTON UNIRSE
  destino_boton.x = ancho * 3/4 - destino_boton.w / 2;
  
  estructura.normal = normal;
  estructura.apretado = apretado;
  estructura.resaltado = sobre;
  estructura.destino = destino_boton;
  
  unirse->asignar_texturas(path + "imagenes/boton.png", estructura, ventana);
  
  escritor->copiar_texto("Unirse", &sup);
    
  destino_texto.x = destino_boton.x + destino_boton.w / 10;
  destino_texto.y = destino_boton.y + destino_boton.h / 10;
  unirse->agregar_texto(&sup, destino_texto, ventana, 1);
  
  return 0;
}

//
bool Sala::correr(Ventana *ventana){
  if (!socket_enviar) return false;
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
    frames.obtener_delay(tiempo_actual, delay);
    
    // Presentar en ventana
    ventana->presentar(delay);
  }
  
  return false;
}

//
int Sala::dibujar(Ventana *ventana){
  fondo->dibujar(ventana);
  if (!seleccionando){
    crear->dibujar(ventana);
    unirse->dibujar(ventana);
  }else{
    seleccion->dibujar(ventana);
  }
  return 0;
}

//
bool Sala::analizar_evento(SDL_Event &evento){
  if (evento.type == SDL_QUIT){
    socket_recibir->cerrar_enviar_recibir();
    seleccion->finalizar_recibir_datos();
    return false;
  }
  if (!seleccionando){
    crear->analizar_evento(evento);
    unirse->analizar_evento(evento);
    if (crear->activado()){
      seleccion->partidas_creadas(false);
      this->enviar_datos(CREAR);
      std::cout << "se envia crear " << (int) CREAR << std::endl;
      seleccionando = true;
    }else if(unirse->activado()){
      seleccion->partidas_creadas(true);
      this->enviar_datos(UNIRSE);
      std::cout << "se envia unirse " << (int) UNIRSE << std::endl;
      seleccionando = true;
    }
  }else{
    int resultado = seleccion->analizar_evento(evento);
    if (resultado == -1) seleccionando = false;
    else if (resultado == 1) return true;
  }
  return true;
}

//
void Sala::enviar_datos(char eleccion){
  socket_enviar->enviar(&eleccion, sizeof(eleccion));
}
