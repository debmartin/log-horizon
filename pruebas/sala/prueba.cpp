#include <iostream>

#include "../../cliente/Sala/sala.h"
#include "../../libs/SDL2/SDL.h"

#define ANCHO 1200
#define ALTO 800

int main(void){
  Socket *socket_receptor = new Socket;
  socket_receptor->asignar_direccion(8010, "127.0.0.1");
/*  if (socket_receptor->conectar() == -1){
    delete socket_receptor;
    return 1;
  }
*/
  Socket *socket_emisor = new Socket;
  socket_emisor->asignar_direccion(8011, "127.0.0.1");
/*  if (socket_emisor->conectar() == -1){
    delete socket_emisor;
    delete socket_receptor;
    return 1;
  }
*/  
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);

  Ventana ventana;
  ventana.abrir(ANCHO,ALTO);
  ventana.dimension_logica(ANCHO, ALTO);
  
  Sala sala;
  sala.inicializar("../../recursos/", &ventana, ANCHO, ALTO, socket_emisor, socket_receptor);
  if (!sala.correr(&ventana)) std::cout << "caca" << std::endl;
  
  return 0;
}
