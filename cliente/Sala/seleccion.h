#ifndef SELECCION_H
#define SELECCION_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"

class Seleccion{
  public:
    //
    Seleccion();
  
    //
    virtual ~Seleccion();
    
    //
    int inicializar(const std::string &path, Ventana *ventana, Socket* enviar, Socket* recibir);
    
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
  private:
    Textura *fondo;
    Boton *aceptar;
    Boton *siguiente;
    Boton *anterior;
    Boton *retroceder;
    Texto *escritor;
    
    Socket* socket_recibir;
    Socket* socket_enviar;
};

#endif // SELECCION_H
