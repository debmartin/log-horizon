#ifndef TEXTO_H
#define TEXTO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "superficie.h"

class Texto{
  public:
    //
    Texto();
    
    //
    ~Texto();
    
    // Setea la fuente a utilizar, abriendo la que recibe como
    // parametro
    bool asignar_fuente(const std::string& path, int font_tam);
    
    // Asigna un color al texto
    void asignar_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha_level);
    
    // Copia el string 
    bool copiar_texto(const std::string& text, Superficie *superficie);
    
    //
    void cerrar_fuente();
    
    //
    void agregar_borde(int espesor);

  private:
    SDL_Color color;
    TTF_Font* fuente;
};

#endif // TEXTO_H
