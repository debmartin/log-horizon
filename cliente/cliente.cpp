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
  puerto = ntohl(puerto);
}

bool conectar(SocketPrefijo *iniciador, SocketPrefijo *receptor, SocketPrefijo *emisor){
  in_addr_t ip = iniciador->ver_ip();
  unsigned int puerto_receptor, puerto_emisor;
  
  iniciador->recibir(&puerto_receptor, sizeof(unsigned int));
  puerto_receptor = ntohl(puerto_receptor);
  iniciador->recibir(&puerto_emisor, sizeof(unsigned int));
  puerto_emisor = ntohl(puerto_emisor);
  
  std::cout << "puerto receptor = " << puerto_receptor << " puerto emisor = " << puerto_emisor << std::endl;
  
  receptor->asignar_direccion(puerto_receptor, ip);
  if (receptor->conectar() == -1) return false;;
  emisor->asignar_direccion(puerto_emisor, ip);
  return emisor->conectar() != -1;  
}

int main(void){
  int ancho, alto, puerto;
  std::string ip;
  leer_datos_iniciales(ancho, alto, ip, puerto);
  
  SocketPrefijo *iniciador = new SocketPrefijo;
  iniciador->asignar_direccion(puerto, ip.c_str());
  if (iniciador->conectar() == -1) return -1;   // AGREGAR MAS PUERTOS O ALGUN WHILE PARA QUE INTENTE VARIAS VECES
  
  SocketPrefijo *receptor = new SocketPrefijo;
  SocketPrefijo *emisor = new SocketPrefijo;
  
  
  if (!conectar(iniciador, receptor, emisor)){
     delete receptor;
     delete emisor;
     return -2;
  }

  delete iniciador;
  
  HDA_Online juego(ancho, alto, emisor, receptor);
  juego.correr();
  
  delete receptor;
  delete emisor;

  return 0;
}
