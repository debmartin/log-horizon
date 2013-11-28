#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "Login/login.h"
#include "Sala/sala.h"
#include "Nivel/nivel.h"
#include "../libs/SDL2/SDL.h"

int main(void){
  Socket *socket_receptor = new Socket;
  socket_receptor->asignar_direccion(8000, "127.0.0.1");
/*  if (socket_receptor->conectar() == -1){
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
  
  std::ifstream resolucion("resolucion.dat", std::fstream::in);
  std::string temp;
  std::getline(resolucion, temp);
  unsigned int ancho = atoi(temp.c_str());
  std::getline(resolucion, temp);
  unsigned int alto = atoi(temp.c_str());
  resolucion.close();
    
  SDL libreria = SDL(SDL_INIT_EVERYTHING);
  libreria.habilitar_texto();
  libreria.habilitar_sonido(44100, MIX_DEFAULT_FORMAT, 2, 4096);
  
  Ventana ventana;
  ventana.abrir(ancho, alto);
  ventana.dimension_logica(ancho, alto);
  std::string titulo = "Hora de Aventura Online";
  ventana.titulo(titulo);
  
  Login *log = new Login;
  log->inicializar("../recursos/", &ventana, ancho, alto, socket_emisor, socket_receptor);
  bool seguir = log->correr(&ventana);
  delete log;
  if (!seguir) return 0;
  
  Sala *sala = new Sala;
  sala->inicializar("../recursos/", &ventana, ancho, alto, socket_emisor, socket_receptor);
  seguir = sala->correr(&ventana);
  if (!seguir){
    delete sala;
    return 0;
  }
  
  // idem a sala pero para nivel
  
  delete sala;
  
  return 0;
}
