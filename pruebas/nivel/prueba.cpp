#include <iostream>

#include "../../libs/SDL2/SDL.h"
#include "../../cliente/Nivel/nivel.h"
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/socket/socket_prefijo.h"

#define ANCHO 800
#define ALTO 600

int main(void){
  SocketPrefijo *socket_receptor = new SocketPrefijo;
  socket_receptor->asignar_direccion(8000, "127.0.0.1");
  if (socket_receptor->conectar() == -1){
    delete socket_receptor;
    return 1;
  }

  SocketPrefijo *socket_emisor = new SocketPrefijo;
  socket_emisor->asignar_direccion(8001, "127.0.0.1");
  if (socket_emisor->conectar() == -1){
    delete socket_emisor;
    delete socket_receptor;
    return 1;
  }
  
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  libreria.habilitar_texto();
  Ventana *ventana = new Ventana;
  ventana->abrir(ANCHO,ALTO);
  ventana->dimension_logica(ANCHO,ALTO);
  
  Nivel *nivel = new Nivel;
  nivel->inicializar("nivel1/", ventana, socket_emisor, socket_receptor);
  nivel->correr(ventana);
  delete nivel;
  delete socket_receptor;
  
  return 0;
}
