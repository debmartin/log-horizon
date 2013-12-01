#ifndef RECEPTOR_PARTIDAS_H
#define RECEPTOR_PARTIDAS_H

#include "../../libs/TDA/socket/socket_prefijo.h"
#include "../../libs/TDA/thread/thread.h"

typedef struct nivel_info{
  std::string nombre_partida;
  std::string nombre_creador;
  uint32_t jugadores_max;
  uint32_t jugadores;
  uint32_t puntaje_max;
}nivel_info_t;

class ReceptorPartidas : public Thread{
  public:
    //
    ReceptorPartidas();
  
    //
    ~ReceptorPartidas();

    //
    void inicializar(SocketPrefijo *receptor);
    
    //
    void partidas_creadas(bool &partida_creada){creada = partida_creada;}
    
    //
    bool esta_creada(){return creada;}
    
    //
    void ver_info(nivel_info_t &nivel);
    
    //
    bool datos_recibidos();
    
    //
    void datos_leidos();
    
  private:
    bool seguir;
    bool datos_nuevos;
    nivel_info_t info;
    
    Mutex mutex;
    bool creada;
    SocketPrefijo *receptor; 
    
  protected:
    //
    void funcion_a_correr();
};

#endif // RECEPTOR_PARTIDAS_H
