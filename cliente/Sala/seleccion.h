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
    int analizar_evento(SDL_Event &evento);
    
    //
    void partidas_creadas(bool es_creada);
    
    //
    void recibir_datos();
    
    //
    void enviar_datos(char eleccion);
    
    //
    void finalizar_recibir_datos();
    
  private:
    Textura *fondo;
    Boton *aceptar;
    Boton *siguiente;
    Boton *anterior;
    Boton *retroceder;
    Boton *iniciar;
    Texto *escritor;
    Partida *partida;
    Socket* socket_enviar;
    bool partida_aceptada;
    bool creador;
};

#endif // SELECCION_H
