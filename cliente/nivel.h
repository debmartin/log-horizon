#ifndef NIVEL_H
#define NIVEL_H

#include "SDL/SDL.h"
#include "matriz.h"
#include "lista.h"
#include <vector>

typedef struct animacion_textura{
  Animacion *animacion;
  Textura *textura;
}animacion_t;

class Explosion{
  public:
    //
    Explosion();
    
    //
    ~Explosion();
    
    //
    void cargar_animacion(const std::string &path, Ventana *ventana);
    
    //
    void explotar(SDL_Rect &celda, Matriz* tablero);
    
    //
    bool explosion_en_curso();
    
    //
    void animar(int fps);
  
    //
    SDL_Rect borrar_primera();
    
    //
    bool finalizada();
    
    //
    bool celdas_vacias();
  
  private:
    Textura *textura;
    Animacion *animacion;
    Lista<SDL_Rect> *celdas;
    bool en_curso;
};

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
    void actualizar_animaciones(int fps);
  
  private:
    int cant_animaciones;
    animacion_t **animaciones;
    Matriz *tablero;
    Explosion *explosion;
    
    Textura *fondo;
    Textura *seleccion;
    Mix_Chunk *sonido;
    
};

#endif // NIVEL_H
