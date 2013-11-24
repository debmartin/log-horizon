#ifndef LOGIN_H
#define LOGIN_H

#include "../../libs/SDL2/SDL.h"

class Login{
  public:
    //
    Login();
  
    //
    ~Login();
    
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
    SDL_Rect destino_usuario;
    SDL_Rect destino_clave;
    
  protected:
    //
    int cargar_archivos(Ventana *ventana, unsigned int ancho, unsigned  int alto);
    
    //
    int dibujar(Ventana *ventana);
    
    //
    bool analizar_evento(SDL_Event &evento);
};

#endif // LOGIN_H
