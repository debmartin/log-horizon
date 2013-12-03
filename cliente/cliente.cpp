#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

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

bool conectar(SocketPrefijo *iniciador, SocketPrefijo *receptor, SocketPrefijo *emisor){
  in_addr_t ip = iniciador->ver_ip();
  unsigned int puerto_receptor, puerto_emisor;
  
  uint32_t largo;
  Json::Value recibido;
  if (receptor->recibir_largo(largo) == 0) return false;
  char *mensaje_char = new char[largo + 1];
  mensaje_char[largo] = '\0';
  if (receptor->recibir(mensaje_char, largo) == 0) return false;
  std::string mensaje_str = std::string(mensaje_char);
  delete[] mensaje_char;
  std::istringstream mensaje_stream(mensaje_str);
  Json::Reader reader;
  reader.parse(mensaje_stream, recibido, false);
  puerto_emisor = recibido["enviar"].asInt();
  puerto_emisor = ntohl(puerto_emisor);
  puerto_receptor = recibido["recibir"].asInt();
  puerto_receptor = ntohl(puerto_receptor);
  
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
  std::cout << "intentando conectar al puerto : " << puerto << std::endl;
  if (iniciador->conectar() == -1){
    std::cout << "error al conectar" << std::endl;
    return -1;   // AGREGAR MAS PUERTOS O ALGUN WHILE PARA QUE INTENTE VARIAS VECES
  }
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
