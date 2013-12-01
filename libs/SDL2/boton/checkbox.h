#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../ventana/ventana.h"
#include "../superficie/superficie.h"
#include "boton.h"


typedef struct estructura_checkbox{
  SDL_Rect normal;
  SDL_Rect apretado;
  SDL_Rect resaltado;
  SDL_Rect resaltado_apretado;
  SDL_Rect destino;
}estructura_checkbox_t;

class CheckBox : public Boton{
  public:
    //
    void analizar_evento(SDL_Event &evento);
    
    //
    bool asignar_texturas(Superficie *sup, estructura_checkbox_t &estructura, Ventana *ventana);
            
    //
    bool asignar_texturas(const std::string &path, estructura_checkbox_t &estructura, Ventana *ventana);
                             
    private:
      SDL_Rect src_apretado_resaltado;
/*  
    //
    CheckBox();
    
    //
    ~CheckBox();
    
    //
    bool agregar_texto(Superficie *sup, SDL_Rect &dest_texto, Ventana *ventana, int desp);
    
    //
    bool dibujar(Ventana *ventana);
    
    //
    bool activado(){return apretado;}
    
    // Setea el nivel alpha de la textura del boton
    void establecer_alpha(Uint8 alpha);
    
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
    int desp;*/
};

#endif // CHECKBOX_H
