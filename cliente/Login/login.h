#ifndef LOGIN_H
#define LOGIN_H

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"

class Login{
  public:
    //
    Login();
  
    //
    ~Login();
    
    //
    void asignar_sockets(Socket* socket_recibir, Socket* socket_enviar);
    
    //
    int correr(Ventana *ventana, unsigned int ancho, unsigned  int alto);
    
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

#endif // LOGIN_H
