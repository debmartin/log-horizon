#ifndef SALA_H
#define SALA_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"
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
    int inicializar(const std::string &path, Ventana *ventana, unsigned int ancho, unsigned int alto, Socket* enviar, Socket* recibir);
    
  private:
    Textura *fondo;
    Boton *crear;
    Boton *unirse;
    Seleccion *seleccion;
    Texto *escritor;
    bool seleccionando;
    Socket* socket_recibir;
    Socket* socket_enviar;
    
  protected:
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    virtual void enviar_datos();
    
    //
    void obtener_delay(FPS &frames, int tiempo_actual, int &delay);
};

#endif // SALA_H
