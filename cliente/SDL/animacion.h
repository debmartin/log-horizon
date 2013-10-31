#ifndef ANIMACION_H
#define ANIMACION_H

#include <SDL2/SDL.h>
#include "ventana.h"
#include "superficie.h"

typedef struct SpritePos{
  SDL_Rect dimension_total;
  SDL_Rect imagen_inicial;
  int desplazamiento_x;
  int desplazamiento_y;
}SpritePos_t;

class Animacion{
  public:
    //
    Animacion();
    
    //
    ~Animacion();
    
    //
    bool cargar_sprite(const std::string &archivo, SpritePos_t &estructura, Ventana* ventana);
    
    //
    bool cargar_sprite(Superficie *sup, SpritePos_t &estructura, Ventana *ventana);
    
    //
    bool dibujar(SDL_Rect &destino, Ventana *ventana);
    
  private:
    Textura *tex;
    SDL_Rect dimension_total;
    SDL_Rect imagen_inicial;
    SDL_Rect imagen_actual;
    int desp_x;
    int desp_y;
};

#endif // ANIMACION_H
