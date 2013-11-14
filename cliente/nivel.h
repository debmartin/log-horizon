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
    void explotar(coordenada_t &celda, Matriz* tablero);
    
    //
    bool explosion_en_curso();
    
    //
    void animar();
  
    //
    coordenada_t borrar_primera();
    
    //
    bool finalizada();
    
    //
    bool celdas_vacias();
  
  private:
    Textura *textura;
    Animacion *animacion;
    Lista<coordenada_t> *celdas;
    bool en_curso;
    
  protected:
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
    void actualizar_animaciones();
  
  private:
    int cant_animaciones;
    animacion_t **animaciones;
    Matriz *tablero;
    Explosion *explosion;
    
    Textura *fondo;
    Textura *seleccion;
    Mix_Chunk *sonido;
    
  protected:
    //
    int calcular_delay(FPS &frames);
    
    //
    void intercambiar(coordenada_t &inicio, coordenada_t &fin);
};

#endif // NIVEL_H
