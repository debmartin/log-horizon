#include <iostream>
#include "receptor_resultados.h"
#include "socket.h"
#include "thread.h"
#include <unistd.h>

int main(void){
  Mutex *mutex = new Mutex;
  Socket *socket = new Socket;
  socket->asignar_direccion(8010, "127.0.0.1");
  if (socket->reusar() == -1) return 1;
  if (socket->asociar() == -1) return 2;
  if (socket->escuchar() == -1) return 3;
  Socket *cliente = new Socket;
  if (socket->aceptar(*cliente) == -1) return 4;
  ReceptorResultados *receptor = new ReceptorResultados;
  receptor->agregar_socket(cliente);
  receptor->agregar_mutex(mutex);
  receptor->correr();
  char contador = 0;
  while(true){
    dato_t a, b;
    if (!receptor->cola_vacia()){
      int c = receptor->borrar_siguiente(a,b);
      std::cout << c << std::endl;
    }
    usleep(1000000);
    contador++;
  }
  return 0;
}
