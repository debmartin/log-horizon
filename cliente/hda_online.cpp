#include <iostream>

#include "cliente.h"
#include "Login/login.h"
#include "Sala/sala.h"
#include "Nivel/nivel.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"

  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  Ventana *ventana = new Ventana;
  ventana->abrir(ancho, alto);
  ventana->dimension_logica(ancho, alto);
  std::string titulo = "Hora de Aventura Online";
  ventana->titulo(titulo);
  
  Login *log = new Login;
  log->inicializar("../recursos/", ventana, ancho, alto, socket_emisor, socket_receptor);
  bool seguir = log->correr(ventana);
  delete log;
  if (!seguir) return 0;
  
  Sala *sala = new Sala;
  sala->inicializar("../recursos/", ventana, ancho, alto, socket_emisor, socket_receptor);
//  Nivel *nivel = new Nivel;
//  nivel->inicializar("../recursos/", ventana, ancho, alto, socket_emisor, socket_receptor);
  while (seguir){
    
    seguir = sala->correr(ventana);
    if (seguir){
//      seguir = nivel->correr(ventana);
    }
  }
  
  delete sala;
//  delete nivel;
  socket_emisor->cerrar_enviar_recibir();
  socket_receptor->cerrar_enviar_recibir();
  delete socket_emisor;
  delete socket_receptor;
  
