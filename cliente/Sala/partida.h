#ifndef PARTIDA_H
#define PARTIDA_H

#include "../../libs/TDA/socket/socket_prefijo.h"
#include "../../libs/SDL2/SDL.h"
#include "receptor_partidas.h"

class Partida{
  public:
    //
    Partida();
  
    //
    ~Partida();
  
    //
    void inicializar(SocketPrefijo *receptor, const std::string &fuente, SDL_Rect &destino, Ventana *ventana);
    
    //
    int dibujar(Ventana *ventana);
    
    //
    void partidas_creadas(bool es_creada);
    
    //
    void finalizar_recibir(){receptor_partidas.join();}
    
    //
    void recibir_datos();
    
    //
    bool recibir_nivel(){return finalizar;}
    
  private:
    Texto *escritor;
    Textura *nombre;
    Textura *puntos;
    Textura *jugadores;
    Textura *creador;
    SDL_Rect destino;
    ReceptorPartidas receptor_partidas;
    Ventana *ventana;
    bool creada;
    bool finalizar;
    
  protected:
    //
    bool actualizar(Ventana *ventana);
    
    //
    std::string itostr(uint32_t valor);
};

#endif // PARTIDA_H
