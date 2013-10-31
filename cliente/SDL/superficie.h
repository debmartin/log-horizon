#ifndef SUPERFICIE_H
#define SUPERFICIE_H
 
#include <SDL2/SDL.h>

/*
 * Previamente debe inicializarse las librerias SDL antes de utulizar 
 * esta clase con la siguiente funcion:
 * 
 *                   int SDL_Init(Uint32 flags)
 * 
 * Así mismo, una vez finalizadas todas las tareas que involucren 
 * a las librerias SDL se debe utilizar la funcion:
 * 
 *                   SDL_Quit()
 */
 
class Superficie{
  public:
    //
    Superficie();
    
    //
    ~Superficie();
    
    //
    bool cargar(const std::string &archivo);
    
    //
    bool color_clave(Uint8 R, Uint8 G, Uint8 B);
    
    //
    bool color_clave(SDL_Color &color);
    
    //
    bool color_pixel(int x, int y, SDL_Color &color);
    
    //
    SDL_Surface* ver_surface(){return surface;}
  private:
    SDL_Surface *surface;
};

#endif // SUPERFICIE_H
