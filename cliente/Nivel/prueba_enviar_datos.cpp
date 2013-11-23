#include <iostream>
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/thread/thread.h"
#include "receptor_resultados.h"
#include <unistd.h>

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
      resultado_t resultado;
      while(true){
        if (cliente->recibir(&resultado, sizeof(resultado_t)) == 0){
          return;
        }
        int a = resultado.tipo;
        int b = resultado.primero.valor1;
        int c = resultado.primero.valor2;
        int d = resultado.segundo.valor1;
        int e = resultado.segundo.valor2;
        
        std::cout << "recibido: " << a << b << c << d << e << std::endl;

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
  resultado_t resultado;
  dato_t dato1, dato2;
  std::string linea;
  while(true){
    std::getline(std::cin, linea);
    if (linea[0] == 'q') return 0;
    resultado.tipo = linea[0] - '0';
    dato1.valor1 =  linea[1] - '0';
    dato1.valor2 = linea[2] - '0';
    dato2.valor1 = linea[3] - '0';
    dato2.valor2 = linea[4] - '0';
    resultado.primero = dato1;
    resultado.segundo = dato2;
    if (cliente->enviar(&resultado, sizeof(resultado_t)) == -1) return 5;
/*    
*/  }
  return 0;
}
