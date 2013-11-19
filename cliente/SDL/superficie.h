#ifndef SUPERFICIE_H
#define SUPERFICIE_H
 
#include <SDL2/SDL.h>
 
class Superficie{
  public:
    // Constructor de clase
    Superficie();
    
    // Destructor de clase
    ~Superficie();
    
    // Carga en memoria el contenido del archivo en la superficie
    bool cargar(const std::string &archivo);
    
    // Escala la superficie a la escala ingresada
    // Post: Se modifico el tamanio de la superficie
    bool escalar(int ancho, int alto);
    
    // Establece el color clave de la superficie
    bool color_clave(Uint8 R, Uint8 G, Uint8 B);
    
    // Establece el color clave de la superficie
    bool color_clave(SDL_Color &color);
    
    // obtiene el color del pixel ingresado y lo almacena en el parametro
    // "color"
    bool color_pixel(int x, int y, SDL_Color &color);
    
    // Devuelve el atributo de la superficie
    SDL_Surface* ver_surface(){return surface;}
  private:
    SDL_Surface *surface;
};

#endif // SUPERFICIE_H
