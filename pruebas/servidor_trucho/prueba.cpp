#include <iostream>
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/socket/socket_prefijo.h"
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(void){
  SocketPrefijo asignador;
  asignador.asignar_direccion(8000, "127.0.0.1");
  asignador.reusar();
  asignador.asociar();
  asignador.escuchar();
  Socket asignador_emisor;
  asignador.aceptar(asignador_emisor);
  
  SocketPrefijo receptor_aux, emisor_aux;
  receptor_aux.asignar_direccion(8010, "127.0.0.1");
  receptor_aux.reusar();
  receptor_aux.asociar();
  receptor_aux.escuchar();
  emisor_aux.asignar_direccion(8011, "127.0.0.1");
  emisor_aux.reusar();
  emisor_aux.asociar();
  emisor_aux.escuchar();
  
  int puerto_emisor = 8011;
  int puerto_receptor = 8010;
  
  asignador_emisor.enviar(&puerto_emisor, sizeof(int));
  asignador_emisor.enviar(&puerto_receptor, sizeof(int));
  
  SocketPrefijo receptor, emisor;
  emisor_aux.aceptar(emisor);
  receptor_aux.aceptar(receptor);
  
  char respuesta = 1; 
  std::string usuario_valido = "Deuteronomio";
  std::string clave_valida = "123qwe";
  while (respuesta == 1){
  
    uint32_t largo;
    receptor.recibir_largo(largo);
    char *usuario = new char[largo + 1];
    usuario[largo] = '\0';
    receptor.recibir(usuario, largo);
    std::cout << "largo = " << largo << " usuario: " << usuario << std::endl;
    
    receptor.recibir_largo(largo);
    char *clave = new char[largo + 1];
    clave[largo] = '\0';
    receptor.recibir(clave, largo);
    std::cout << "largo = " << largo << " clave: " << clave << std::endl;
    
    if (strcmp(usuario, usuario_valido.c_str()) == 0 && strcmp(clave, clave_valida.c_str()) == 0)
      respuesta = 0;
    
    delete[] usuario;
    delete[] clave;

    emisor.enviar(&respuesta, sizeof(char));
  }
  
  return 0;
}
