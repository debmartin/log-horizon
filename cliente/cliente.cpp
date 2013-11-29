#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "cliente.h"
#include "Login/login.h"
#include "Sala/sala.h"
#include "Nivel/nivel.h"
#include "../libs/json/include/json/json.h"
#include "../libs/SDL2/SDL.h"

#define CONFIG "config.dat"

void leer_datos_iniciales(int &ancho, int &alto, std::string &ip, int &puerto){
  Json::Value datos;
  Json::Value aux;
  Json::Reader reader;
  
  std::string configuracion = CONFIG;
  std::ifstream archivo_datos(configuracion.c_str(), std::ifstream::binary | std::ifstream::in);
  reader.parse(archivo_datos, datos, false);
  
  aux = datos.get("ancho", aux);
  ancho = aux.asInt();
  aux = datos.get("alto", aux);
  alto = aux.asInt();
  aux = datos.get("ip", aux);
  ip = aux.asString();
  aux = datos.get("puerto", aux);
  puerto = aux.asInt();
}

bool conectar(Socket *iniciador, Socket *a_conectar){
  in_addr_t ip = iniciador->ver_ip();
  iniciador->recibir(&puerto, sizeof(int));
  a_conectar->asignar_direccion(puerto, ip);
  return a_conectar->conectar() != -1;
}

int main(void){
  int ancho, alto, puerto;
  std::string ip;
  leer_datos_iniciales(ancho, alto, ip, puerto);
  
  if (iniciador->conectar() == -1) return -1;
  Socket *iniciador = new Socket;
  Socket *receptor = new Socket;
  Socket *emisor = new Socket;
  iniciador->asignar_direccion(puerto, ip);
  if (!conectar(iniciador, receptor)) return -2;
  if (!conectar(iniciador, emisor)) return -3;
  
  delete iniciador;
  
  HDA_Online juego(alto, ancho, receptor, emisor);
  juego.correr();
  
  delete receptor;
  delete emisor;

  return 0;
}
