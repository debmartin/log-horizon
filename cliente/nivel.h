#ifndef NIVEL_H
#define NIVEL_H

#include "SDL/SDL.h"
#include "matriz.h"
#include <vector>

typedef struct animacion_textura{
  Animacion *animacion;
  Textura *textura;
  int contador;
}animacion_t;

class Nivel{
  public:
    //
    Nivel();
    
    //
    ~Nivel();
    
    //
    void correr(const std::string &path, Ventana* ventana);
  
  protected:
    //
    void inicializar_datos(const std::string &path, Ventana *ventana);
    
    //
    void dibujar(Ventana *ventana);
  
    //
    bool analizar_evento(SDL_Event &evento);
    
    //
    void actualizar_animaciones();
  
  private:
    int cant_animaciones;
    animacion_t **animaciones;
    Textura *fondo;
    Textura *seleccion;
    animacion_t *explosion;
    Matriz *tablero;
    bool en_explosion;
    bool explosion_finalizada;
    int contador_animacion;
    int contador_explosion;
    std::vector<SDL_Rect> celdas_explotar;
    Mix_Chunk *sonido;
};

#endif // NIVEL_H
