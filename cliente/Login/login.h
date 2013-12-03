#ifndef LOGIN_H
#define LOGIN_H

#include <string>

#include "../../libs/SDL2/SDL.h"
#include "../../libs/TDA/socket/socket.h"
#include "../../libs/TDA/socket/socket_prefijo.h"
#include "autentificador.h"

class Login{
  public:
    //
    Login();
  
    //
    virtual ~Login();
    
    //
    bool correr(Ventana *ventana);
    
    //
    int inicializar(const std::string &path, Ventana *ventana, 
                    SocketPrefijo* enviar, SocketPrefijo* recibir);
    
  private:
    bool datos_inicializados;
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
    Autentificador *autentificador;    
    
  protected:
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void enviar_datos();
};

#endif // LOGIN_H
