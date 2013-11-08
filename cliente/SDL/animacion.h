#ifndef ANIMACION_H
#define ANIMACION_H

#include "SDL.h"
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
    void cargar_sprite(SpritePos_t &estructura);
    
    //
    bool dibujar(Textura* tex, SDL_Rect &destino, Ventana *ventana);
    
    //
    bool siguiente();
    
    //
    bool anterior();
    
    //
    void reiniciar();
    
    //
    bool fuera_del_sprite();
    
    //
    bool al_inicio();
    
  private:
    SDL_Rect dimension_total;
    SDL_Rect imagen_inicial;
    SDL_Rect imagen_actual;
    int desp_x;
    int desp_y;
};


class AnimacionMovil{
  public:
    //
    AnimacionMovil();
    
    //
    void asignar_animacion(Animacion *anim, Textura *tex);
    
    //
    void posicion_actual(SDL_Rect &pos);
    
    //
    void mover(SDL_Rect &destino);
    
    //
    SDL_Rect ver_posicion_actual();
    
    //
    SDL_Rect ver_posicion_destino();
    
    //
    bool movimientos_pendientes();
    
    //
    bool dibujar(Ventana *ventana);
    
    //
    void quitar_textura();
    
    //
    bool al_inicio();
    
  private:
    Textura *textura;
    Animacion *animacion;
    SDL_Rect pos_actual;
    SDL_Rect pos_destino;
};

#endif // ANIMACION_H
