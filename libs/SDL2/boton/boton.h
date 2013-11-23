#ifndef BOTON_H
#define BOTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../animacion/animacion.h"
#include "../ventana/ventana.h"
#include "../superficie/superficie.h"

typedef struct estructura_boton{
  SDL_Rect normal;
  SDL_Rect apretado;
  SDL_Rect resaltado;
  SDL_Rect destino;
}estructura_boton_t;

class Boton{
  public:
    //
    Boton();
    
    //
    ~Boton();
    
    //
    bool asignar_texturas(Superficie *sup, estructura_boton_t &estructura, Ventana *ventana);
                             
    //
    bool asignar_texturas(const std::string &path, estructura_boton_t &estructura, Ventana *ventana);
    
    //
    bool agregar_texto(Superficie *sup, SDL_Rect &dest_texto, Ventana *ventana, int desp);
    
    //
    void analizar_evento(SDL_Event &evento);
    
    //
    bool dibujar(Ventana *ventana);
    
    //
    bool activado(){return apretado;}
  private:
    Textura *textura;
    SDL_Rect src_normal;
    SDL_Rect src_apretado;
    SDL_Rect src_resaltado;
    SDL_Rect src_actual;
    Textura *texto;
    SDL_Rect destino_texto;
    bool apretado;
    SDL_Rect destino;
    int desp;
};

#endif // BOTON_H
