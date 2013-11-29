#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "hda_online.h"
#include "../libs/TDA/socket/socket_prefijo.h"
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

bool conectar(SocketPrefijo *iniciador, SocketPrefijo *a_conectar){
  in_addr_t ip = iniciador->ver_ip();
  unsigned int puerto;
  iniciador->recibir(&puerto, sizeof(int));
  a_conectar->asignar_direccion(puerto, ip);
  return a_conectar->conectar() != -1;
}

int main(void){
  int ancho, alto, puerto;
  std::string ip;
  leer_datos_iniciales(ancho, alto, ip, puerto);
  
  SocketPrefijo *iniciador = new SocketPrefijo;
  iniciador->asignar_direccion(puerto, ip.c_str());
  if (iniciador->conectar() == -1) return -1;   // AGREGAR MAS PUERTOS O ALGUN WHILE PARA QUE INTENTE VARIAS VECES
  
  SocketPrefijo *receptor = new SocketPrefijo;
  
  if (!conectar(iniciador, receptor)){
     delete receptor;
     return -2;
  }
  SocketPrefijo *emisor = new SocketPrefijo;
  if (!conectar(iniciador, emisor)){
    delete emisor;
    delete receptor;
    return -3;
  }
  
  delete iniciador;
  
  HDA_Online juego(ancho, alto, emisor, receptor);
  juego.correr();
  
  delete receptor;
  delete emisor;

  return 0;
}
