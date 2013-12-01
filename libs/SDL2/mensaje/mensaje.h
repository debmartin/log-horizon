#ifndef MENSAJE_H
#define MENSAJE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../ventana/ventana.h"
#include "../superficie/superficie.h"
#include "../texto/texto.h"

class Mensaje{
  public:
    //
    Mensaje();
    
    //
    ~Mensaje();
    
    //
    void asignar_escritor(const std::string& fuente, int font_tam);
    
    //
    void asignar_fondo(Superficie *sup, SDL_Rect &destino, Ventana *ventana);
    
    //
    void asignar_fondo(const std::string &path, SDL_Rect &destino, Ventana *ventana);
                             
    //
    void asignar_mensaje(Superficie *sup, SDL_Rect &destino, Ventana *ventana);
    
    //
    void asignar_mensaje(const std::string &mensaje, SDL_Rect &destino, Ventana *ventana);
    
    //
    void  establecer_alpha_fondo(Uint8 alpha);
    
    //
    void dibujar(Ventana *ventana);
   
  private:
    Textura *fondo;
    SDL_Rect destino_fondo;
    Textura *mensaje;
    SDL_Rect destino_mensaje;
    Texto *escritor;
};

#endif // MENSAJE_H
