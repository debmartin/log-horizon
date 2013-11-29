#include <iostream>
#include "../../libs/TDA/socket/socket.h"
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main(void){
  Socket asignador;
  asignador.asignar_direccion(8000, "127.0.0.1");
  asignador.reusar();
  asignador.asociar();
  asignador.escuchar();
  Socket asignador_emisor;
  asignador.aceptar(asignador_emisor);
  
  Socket receptor_aux, emisor_aux;
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
  
  Socket receptor, emisor;
  emisor_aux.aceptar(emisor);
  receptor_aux.aceptar(receptor);
  
  char respuesta = 1; 
  while (respuesta == 1){
  std::string usuario_valido = "Deuteronomio";
  std::string clave_valida = "123qwe";
  unsigned int largo_usuario;
  unsigned int largo_clave;
  
    receptor.recibir(&largo_usuario, sizeof(int));
    char *usuario = new char[largo_usuario + 1];
    usuario[largo_usuario] = '\0';
    receptor.recibir(usuario, largo_usuario);
    std::cout << largo_usuario << std::endl;
    std::cout << "usuario: " << usuario << std::endl;
    
    receptor.recibir(&largo_clave, sizeof(int));
    char *clave = new char[largo_clave + 1];
    clave[largo_clave] = '\0';
    receptor.recibir(clave, largo_clave);
    std::cout << largo_clave << std::endl;
    std::cout << "clave: " << clave << std::endl;
    
    if (strcmp(usuario, usuario_valido.c_str()) == 0 && strcmp(clave, clave_valida.c_str()) == 0)
      respuesta = 0;
    
    delete[] usuario;
    delete[] clave;

    emisor.enviar(&respuesta, sizeof(char));
  }
  
  return 0;
}
