#ifndef ANIMACION_H
#define ANIMACION_H

#include <SDL2/SDL.h>
#include "../ventana/ventana.h"
#include "../superficie/superficie.h"

typedef struct SpritePos{
  SDL_Rect dimension_total;
  SDL_Rect imagen_inicial;
  int desplazamiento_x;
  int desplazamiento_y;
}SpritePos_t;

class Animacion{
  public:
    // constructor de clase
    Animacion();
    
    // establece la posicion origen de la imagen inicial, el desplazamiento
    // para la siguiente imagen y el limite del sprite.
    void cargar_sprite(SpritePos_t &estructura);
    
    // Dibuja en la ventana ingresada en el rectangulo destino la
    // textura, con el origen correspondiente.
    // Pre: la textura y la ventana deben existir
    // Post: se dibujo la textura en la ventana en la posicion destino 
    bool dibujar(Textura* tex, SDL_Rect &destino, Ventana *ventana);
    
    // Ubica la imagen actual en la siguiente posicion
    bool siguiente();
    
    // Ubica la imagen actual en la posicion anterior
    bool anterior();
    
    // Ubica la imagen actual en la posicion inicial
    void reiniciar();
    
    // Devuelve true en caso de que la animacion se encuentre fuera
    // del sprite, caso contrario devolvera false.
    // Para volver a ubicarla dentro del sprite se puede usar los
    // metodos anterior o reiniciar.
    bool fuera_del_sprite();
    
    // devuelve true si la imagen actual tiene la misma ubicacion
    // que la imagen inicial.
    bool al_inicio();
    
    // devuelve true si la imagen actual se encuentra en la ultima
    // imagen, antes de quedar fuera del sprite
    bool al_final();
    
    // actualiza la imagen actual, a la siguiente posicion, dependiend
    // de los fps ingresados previamente
    void animar();
    
    // establece los fps de la animacion, para poder variar de imagen
    // en el ritmo establecido
    void establecer_fps(int fps);
    
  private:
    SDL_Rect dimension_total;
    SDL_Rect imagen_inicial;
    SDL_Rect imagen_actual;
    int desp_x;
    int desp_y;
    Uint32 frame_rate;
    Uint32 tiempo;
};


class AnimacionMovil{
  public:
    // Constructor de clase
    AnimacionMovil();
    
    // Asigna la animacion con su respectiva textura.
    void asignar_animacion(Animacion *anim, Textura *tex);
    
    // Establece la posicion actual en donde será dibujada la animacion
    void posicion_actual(SDL_Rect &pos);
    
    // Establece un destino para la animacion, cada vez qeu se la dibuje
    // se desplazara una cantidad de pixeles igual a salto hacia el destino
    void mover(SDL_Rect &destino, int salto);
    
    // Devuelve la ubicacion actual de la animacion
    SDL_Rect ver_posicion_actual();
    
    // Devuelve el destino de la animacion
    SDL_Rect ver_posicion_destino();
    
    // Devuelve true si la animacion se esta moviendo
    bool movimientos_pendientes();
    
    // Dibuja la animacion en la ventana, en la correspondiente ubicacion
    // actual.
    bool dibujar(Ventana *ventana);
    
    // Asigna NULL al atributo textura, en este caso al usarse el metodo
    // dibujar en la clase, no se dibujara nada.
    void quitar_textura();
    
  private:
    Textura *textura;
    Animacion *animacion;
    SDL_Rect pos_actual;
    SDL_Rect pos_destino;
    int distancia;
};

#endif // ANIMACION_H
