#ifndef SELECCION_H
#define SELECCION_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"
#include "partida.h"

class Seleccion{
  public:
    //
    Seleccion();
  
    //
    virtual ~Seleccion();
        
    //
    int inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir);
    
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void esta_creada(bool es_creada);
    
    //
    void enviar_datos(char eleccion);
    
  private:
    Textura *fondo;
    Boton *aceptar;
    Boton *siguiente;
    Boton *anterior;
    Boton *retroceder;
    Texto *escritor;
    Partida *partida;
    Socket* socket_enviar;
    bool partida_aceptada;
};

#endif // SELECCION_H
