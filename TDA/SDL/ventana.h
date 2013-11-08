#ifndef VENTANA_H
#define VENTANA_H

#include <SDL2/SDL.h>
#include "superficie.h"

/*
 * Previamente debe inicializarse las librerias SDL antes de utulizar 
 * esta clase con la siguiente funcion:
 * 
 *                   int SDL_Init(Uint32 flags)
 * 
 * Así mismo, una vez finalizadas todas las tareas que involucren 
 * a las librerias SDL se debe utilizar la funcion:
 * 
 *                   void SDL_Quit()
 */
 
class Textura;
 
class Ventana{
  public:
    // 
    Ventana();
    
    // 
    ~Ventana();
  
    //
    bool abrir(const int ancho, const int alto);
    
    //
    bool dimension_logica(const int ancho, const int alto);
    
    //
    void titulo(std::string &titulo);
    
    //
    bool limpiar();
    
    //
    bool presentar(const unsigned int delay);
    
    //
    SDL_Renderer *ver_renderer(){return renderer;}
  private:
    SDL_Renderer *renderer;
    SDL_Window *ventana;
};


class Textura{
  public:
    // Constructor de la clase
    Textura();

    // Destructor de la clase
    ~Textura();

    // 
    bool cargar_textura(const std::string archivo, Ventana *ventana);
    
    //
    bool cargar_textura(Superficie *sup , Ventana *ventana);
      
    // 
    bool dibujar(SDL_Rect &origen, SDL_Rect &destino, Ventana *ventana);

    // 
    bool dibujar(SDL_Rect &destino, Ventana *ventana);

    // 
    bool dibujar(Ventana *ventana);
          
  private:
    SDL_Texture *tex;
};

#endif // VENTANA_H
