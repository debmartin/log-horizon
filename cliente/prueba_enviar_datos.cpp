#include <iostream>
#include "socket.h"
#include "thread.h"
#include "receptor_resultados.h"
#include <unistd.h>


int main(void){
  Socket *socket = new Socket;
  socket->asignar_direccion(8010, "127.0.0.1");
  if (socket->reusar() == -1) return 1;
  if (socket->asociar() == -1) return 2;
  if (socket->escuchar() == -1) return 3;
  Socket *cliente = new Socket;
  if (socket->aceptar(*cliente) == -1) return 4;
  
  resultado_t resultado;
  dato_t dato1, dato2;
  std::string linea;
  while(true){
    std::getline(std::cin, linea);
    resultado.tipo = linea[0] - '0';
    dato1.valor1 =  linea[1] - '0';
    dato1.valor2 = linea[2] - '0';
    dato2.valor1 = linea[3] - '0';
    dato2.valor2 = linea[4] - '0';
    resultado.primero = dato1;
    resultado.segundo = dato2;
    if (cliente->enviar(&resultado, sizeof(resultado_t)) == -1) return 5;
  }
  return 0;
}
