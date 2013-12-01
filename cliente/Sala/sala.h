#ifndef SALA_H
#define SALA_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/socket/socket_prefijo.h"
#include "seleccion.h"

class Sala{
  public:
    //
    Sala();
  
    //
    virtual ~Sala();
    
    //
    bool correr(Ventana *ventana);
    
    //
    int inicializar(const std::string &path, Ventana *ventana, SocketPrefijo* enviar, SocketPrefijo* recibir);
    
  private:
    Textura *fondo;
    Boton *crear;
    Boton *unirse;
    Seleccion *seleccion;
    Texto *escritor;
    bool seleccionando;
    SocketPrefijo* socket_enviar;
    
  protected:
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    virtual void enviar_datos(char dato);
};

#endif // SALA_H
