#ifndef PUNTAJE_H
#define PUNTAJE_H

#include "../../libs/SDL2/SDL.h"

class Puntaje{
  public:
    //
    Puntaje();
    
    //
    ~Puntaje();
    
    //
    bool asignar_fuente(const std::string& path, int font_tam, SDL_Color &color);
    
    //
    bool asignar_destino(SDL_Rect &nuevo_destino, Ventana *ventana);
    
    //
    void sumar_puntos(unsigned int dif);
    
    //
    bool dibujar();
    
  private:
    unsigned int puntaje;
    Texto *dibujador_texto;
    Textura *textura_puntaje;
    SDL_Rect *destino;
    Ventana *ventana;
    size_t ancho_caracter;
    
  protected:
    //
    void actualizar_textura();
};

#endif // PUNTAJE_H
