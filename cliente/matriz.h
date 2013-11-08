#ifndef MATRIZ_H
#define MATRIZ_H

#include "SDL/SDL.h"

class Celda{
  public:
    //
    Celda();
    
    //
    ~Celda();
    
    //
    void asignar_animacion(Textura *tex, SpritePos_t &estructura);
    
    //
    void seleccionar(Textura *tex);
    
    //
    void quitar_seleccion();
    
    //
    void asignar_posicion(SDL_Rect &rect);
    
    //
    bool intercambiar(Celda* otra_celda);
    
    //
    bool dibujar(Ventana* ventana);
    
    //
    void quitar_textura();
    
    //
    void mover(SDL_Rect &destino);
    
    //
    void asignar_animacion(Textura *tex, Animacion *anim);
    
    //
    bool movimientos_pendientes();
    
    //
    bool seleccionada();
    
  private:
    AnimacionMovil* animacion;
    Textura *seleccion;
    SDL_Rect seleccion_src;
};

class Matriz{
  public:
    //
    Matriz();
    
    //
    ~Matriz();
    
    //
    void definir_forma(char **estructura, SDL_Rect &dimension, SDL_Rect &celda);
    
    //
    void insertar(Textura *tex, Animacion *anim, SDL_Rect &celda);
    
    //
    void seleccionar(Textura *tex, SDL_Rect &celda);
    
    //
    bool intercambiar(SDL_Rect &celda1, SDL_Rect &celda2);
    
    //
    bool apilar(Textura *tex, Animacion *anim, SDL_Rect &celda);
    
    //
    bool dibujar_fondo_celdas(Superficie *fondo_celda, SDL_Rect *sourc, Superficie *fondo);
    
    //
    bool dibujar(Ventana *ventana);
    
    //
    bool dibujar_celda(Textura *tex, SDL_Rect &celda, Ventana* ventana);
    
    //
    void quitar_seleccion();
    
    //
    bool esta_ocupada();
    
    //
    bool adyacente_seleccionado(SDL_Rect &celda,SDL_Rect &celda_adyacente);
    
  private:
    Celda ***celdas;
    unsigned int filas;
    unsigned int columnas;
    SDL_Rect primer_celda;
    bool ocupada;
    SDL_Rect *celda_seleccionada;
};

#endif // MATRIZ_H
