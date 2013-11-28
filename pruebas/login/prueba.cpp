#include <iostream>

#include "../../cliente/Login/login.h"
#include "../../libs/SDL2/SDL.h"

#define ANCHO 1200
#define ALTO 800

int main(void){
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  Ventana ventana;
  ventana.abrir(ANCHO,ALTO);
  ventana.dimension_logica(ANCHO, ALTO);
  
  Socket *socket_receptor = new Socket;
  socket_receptor->asignar_direccion(8000, "127.0.0.1");
 /* if (socket_receptor->conectar() == -1){
    delete socket_receptor;
    return 1;
  }
*/
  Socket *socket_emisor = new Socket;
  socket_emisor->asignar_direccion(8001, "127.0.0.1");
/*  if (socket_emisor->conectar() == -1){
    delete socket_emisor;
    delete socket_receptor;
    return 1;
  }
 */ 
  Login log;
  log.inicializar("../../recursos/", &ventana, ANCHO, ALTO, socket_emisor, socket_receptor);
  log.correr(&ventana);
  
  return 0;
}
