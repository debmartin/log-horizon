#ifndef TEXTURA_H
#define TEXTURA_H
 
#include <SDL2/SDL.h>
 
class Textura{
  public:
    // Constructor de la clase
    Textura();

    // Destructor de la clase
    ~Textura();

    // Carga la textura del archivo, cuyo nombre es ingresado por
    // parametro.
    // Pre: El renderer debe existir
    // Post: Se guarda la 
    bool cargar_textura(char* archivo, SDL_Renderer *render);
    
    //
    bool establecer_color_clave(

    //
    void seleccionar(int x, int y, int w, int h);

    //
    bool dibujar(int x, int y, int w, int h, SDL_Renderer *render);
  
    //
    bool dibujar(SDL_Renderer *render);
  
  private:
    SDL_Texture *tex;
    SDL_Rect *seleccion;
};
 
#endif


http://www.sdltutorials.com/sdl-coordinates-and-blitting
