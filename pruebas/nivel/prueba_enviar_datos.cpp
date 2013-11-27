#include <iostream>
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/thread/thread.h"
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

class Socket_receptor : public Thread{
  public:
    void funcion_a_correr(){
      sock = new Socket;
      sock->asignar_direccion(8001, "127.0.0.1");
      sock->reusar();
      sock->asociar();
      sock->escuchar();
      cliente = new Socket;
      sock->aceptar(*cliente);
      char resultado[5];
      while(true){
        if (cliente->recibir(&resultado[0], 5) == 0){
          return;
        }
      }
    }
  
  private:
    Socket *sock;
    Socket *cliente;
    Mutex *mu;
};


int main(void){

  Socket_receptor *receptor = new Socket_receptor;
  receptor->correr();
  
  Socket *socket = new Socket;
  socket->asignar_direccion(8000, "127.0.0.1");
  if (socket->reusar() == -1) return 1;
  if (socket->asociar() == -1) return 2;
  if (socket->escuchar() == -1) return 3;
  Socket *cliente = new Socket;
  if (socket->aceptar(*cliente) == -1) return 4;
  std::string linea;
  while(true){
    std::getline(std::cin, linea);
    if (linea[0] == 'q') return 0;
    linea[0] -= '0';
    if (linea[0] != 3){
      linea[1] -= '0';
      linea[2] -= '0';
      linea[3] -= '0';
      linea[4] -= '0';
      if (cliente->enviar(&linea[0], 5) == -1) return 5;
    }else{
      char aux[5];
      memcpy(&aux[0], &linea[1], 4);
      aux[4] = '\0';
      uint32_t puntos_aux = atoi(aux);
      puntos_aux = htonl(puntos_aux);
      memcpy(&linea[1], &puntos_aux, 4);
      if (cliente->enviar(&linea[0], 5) == -1) return 6;
    }
  }
  return 0;
}
