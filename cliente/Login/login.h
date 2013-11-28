#ifndef LOGIN_H
#define LOGIN_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"

class Login{
  public:
    //
    Login();
  
    //
    virtual ~Login();
    
    //
    bool correr(Ventana *ventana);
    
    //
    int inicializar(const std::string &path, Ventana *ventana, unsigned int ancho, unsigned int alto, Socket* enviar, Socket* recibir);
    
  private:
    Textura *fondo;
    Boton *iniciar_sesion;
    TextBox *usuario;
    TextBox *clave;
    Mix_Chunk *sonido;
    Texto *escritor;
    Textura *textura_usuario;
    Textura *textura_clave;
    SDL_Rect destino_text_usuario;
    SDL_Rect destino_text_clave;
    Mensaje *mensaje;
    bool conectando;
    
    Socket* socket_recibir;
    Socket* socket_enviar;
    
  protected:
    
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    virtual bool enviar_datos();
    
    //
    void obtener_delay(FPS &frames, int tiempo_actual, int &delay);
};

#endif // LOGIN_H
