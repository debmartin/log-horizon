#ifndef AUTENTIFICADOR_H
#define AUTENTIFICADOR_H

#include <string>

#include "../../libs/TDA/thread/thread.h"
#include "../../libs/TDA/socket/socket_prefijo.h"

class Autentificador : public Thread{
  public:
    //
    Autentificador();
    
    //
    ~Autentificador();
    
    //
    void asignar_sockets(SocketPrefijo *enviar, SocketPrefijo *recibir);
    
    //
    void autentificar(const std::string &usuario, 
                      const std::string &clave);
    
    //
    bool en_curso();
    
    //
    bool usuario_valido();
    
  private:
    SocketPrefijo *emisor;
    SocketPrefijo *receptor;
    std::string usuario;
    std::string clave;
    bool enviando;
    bool validacion;
    Mutex mutex;
  
  protected:
    //
    void funcion_a_correr();    
};

#endif // AUTENTIFICADOR_H
