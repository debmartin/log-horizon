#include <iostream>
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/thread/thread.h"
#include "../../libs/TDA/socket/socket_prefijo.h"
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define CREAR 0
#define UNIRSE 1
#define SALIRSE -1
#define RETROCEDER 5
#define OK 6
#define ERROR 7


class EnviarNivel : public Thread{
  public:
    void asignar_sockets(SocketPrefijo &emisor_){
      emisor = emisor_;
    }
    
    void enviar_mensaje(char mensaje_){
      mutex.bloquear();
      mensaje = mensaje_;
      std::cout << "mensaje ahora es " << (int) mensaje << std::endl;
      mutex.desbloquear();
    }
    
  void esta_creada(bool creada_){
    mutex.bloquear();
    creada = creada_;
    mutex.desbloquear();
  }
  protected:
    void funcion_a_correr(){
      
      std::string partida = "Locuras en el Dulce Reino";
      uint32_t puntos = 2000;
      uint32_t max_pjs = 5;
      std::string creador = "Mingo";
      uint32_t pjs = 1;

      mensaje = OK;
      while (true){
        mutex.bloquear();
        std::cout << "se envia de mensaje " << (int) mensaje << std::endl;
        int caca = emisor.enviar(&mensaje, sizeof(char));
        std::cout << "caca = " << caca << std::endl;
         
        if (mensaje == ERROR) return;
        mutex.desbloquear();
        
        
        emisor.enviar_con_prefijo(partida.c_str(), partida.size());
        emisor.enviar(&puntos, sizeof(puntos));
        emisor.enviar(&max_pjs, sizeof(max_pjs));
        emisor.enviar(&pjs, sizeof(pjs));
        mutex.bloquear();
        if (creada){
          mutex.desbloquear();
          emisor.enviar_con_prefijo(creador.c_str(), creador.size());
        }else{
          mutex.desbloquear();
        }

        puntos++;
        max_pjs++;
        pjs++;

        sleep(5);
        
      }
    }      
  
  private:
    SocketPrefijo emisor;
    Mutex mutex;
    bool creada;
    char mensaje;
};

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
  puerto_emisor = htonl(puerto_emisor);
  int puerto_receptor = 8010;
  puerto_receptor = htonl(puerto_receptor);
  
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
  
  EnviarNivel envio;
  envio.asignar_sockets(emisor);
  
  char eleccion;
  receptor.recibir(&eleccion, sizeof(char));
  if (eleccion == CREAR){
    envio.esta_creada(false);
  }else if(eleccion == UNIRSE){
    envio.esta_creada(true);
  }else if(eleccion == SALIRSE){
    char c = ERROR;
    emisor.enviar(&c,sizeof(char) );
    return 0;
  }
  envio.correr();
  
  bool lala = true;
  while (lala){
    int caca = receptor.recibir(&eleccion, sizeof(char));
    if (caca == 0){
      lala = false;
      continue;
    }
    std::cout << "caca 2 = " << caca << std::endl;
    std::cout << "se recibio " << (int) eleccion << std::endl;
    if (eleccion == CREAR){
      envio.esta_creada(false);
    }else if(eleccion == UNIRSE){
      envio.esta_creada(true);
    }else if(eleccion == SALIRSE){
      envio.enviar_mensaje(ERROR);
      lala = false;
    }
  }
  
  std::cout << "antes del join" << std::endl;
  emisor.cerrar_enviar_recibir();
  receptor.cerrar_enviar_recibir();
  envio.join();
  
  

  return 0;
}
