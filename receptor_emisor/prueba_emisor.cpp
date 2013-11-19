#include <iostream>
#include "emisor_resultados.h"
#include "socket.h"
#include "thread.h"
#include <unistd.h>

int main(void){
  Mutex *mutex = new Mutex;
  Socket *socket = new Socket;
  socket->asignar_direccion(8010, "127.0.0.1");
  if (socket->conectar() == -1) return 1;
  
  EmisorResultados *emisor = new EmisorResultados;
  emisor->agregar_socket(socket);
  emisor->agregar_mutex(mutex);
  emisor->correr();
  char contador = 0;

  while(true){
    int a = contador;
    std::cout << a << std::endl;
    dato_t primero, segundo;
    primero.valor1 = 1;
    primero.valor2 = 2;
    segundo.valor1 = 3;
    segundo.valor2 = 4;
    
    emisor->encolar_dato(contador, primero, segundo);
    usleep(1000000);
    contador++;
  }
  return 0;
}
