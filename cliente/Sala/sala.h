#ifndef SALA_H
#define SALA_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"

class Sala{
  public:
    //
    Sala();
  
    //
    ~Sala();
    
    //
    void asignar_sockets(Socket* socket_recibir, Socket* socket_enviar);
    
    //
    int correr(Ventana *ventana, unsigned int ancho, unsigned  int alto);
    
  private:
    Textura *fondo;
    Boton *crear;
    Boton *unirse;
    Texto *escritor;
    
    Socket* recibir;
    Socket* enviar;
    
  protected:
    //
    int cargar_archivos(Ventana *ventana, unsigned int ancho, unsigned  int alto);
    
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
