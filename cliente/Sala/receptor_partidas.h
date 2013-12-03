#ifndef RECEPTOR_PARTIDAS_H
#define RECEPTOR_PARTIDAS_H

#include "../../libs/TDA/socket/socket_prefijo.h"
#include "../../libs/TDA/thread/thread.h"
#include "../../libs/json/include/json/json.h"

class ReceptorPartidas : public Thread{
  public:
    //
    ReceptorPartidas();
  
    //
    ~ReceptorPartidas();

    //
    void inicializar(SocketPrefijo *receptor, char finalizar);
    
    //
    void ver_info(Json::Value &mensaje_recibido);
    
    //
    bool datos_recibidos();
    
  private:
    bool seguir;
    bool datos_nuevos;
    Json::Value recibido;
    Mutex mutex;
    SocketPrefijo *receptor; 
    char tipo_finalizador;
    
  protected:
    //
    void funcion_a_correr();
};

#endif // RECEPTOR_PARTIDAS_H
