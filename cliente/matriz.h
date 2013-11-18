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
    bool intercambiar(Celda* otra_celda, int salto);
    
    //
    bool dibujar(Ventana* ventana);
    
    //
    void quitar_textura();
    
    //
    void mover(SDL_Rect &destino, int salto);
    
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

typedef struct coordenada{
  int x;
  int y;
}coordenada_t;

class Matriz{
  public:
    //
    Matriz();
    
    //
    ~Matriz();
    
    //
    void definir_forma(char **estructura, coordenada_t &dimension, SDL_Rect &celda);
    
    //
    void insertar(Textura *tex, Animacion *anim, coordenada_t &celda);
    
    //
    void seleccionar(Textura *tex, coordenada_t &celda);
    
    //
    bool intercambiar(coordenada_t &celda1, coordenada_t &celda2);
    
    //
    bool apilar(Textura *tex, Animacion *anim, coordenada_t &celda);
    
    //
    bool dibujar_fondo_celdas(Superficie *fondo_celda, SDL_Rect *sourc, Superficie *fondo);
    
    //
    bool dibujar(Ventana *ventana);
        
    //
    void quitar_seleccion();
    
    //
    bool esta_ocupada();
    
    //
    bool adyacente_seleccionado(coordenada_t &celda,coordenada_t &celda_adyacente);
    
    //
    int numero_filas(){return filas;}
    
    //
    int numero_columnas(){return columnas;}
    
  private:
    Celda ***celdas;
    int filas;
    int columnas;
    SDL_Rect primer_celda;
    bool ocupada;
    coordenada_t *celda_seleccionada;
};

#endif // MATRIZ_H
