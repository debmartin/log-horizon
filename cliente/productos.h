#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#include "SDL/SDL.h"
#include "matriz.h"

typedef struct animacion_textura{
  Animacion *animacion;
  Textura *textura;
}animacion_t;

class Productos{
  public:
    //
    Productos();
    
    //
    ~Productos();
    
    //
    void cargar_animaciones(const std::string &path, Ventana *ventana);
      
    //
    void animar();
    
    //
    void dibujar(int tipo, int color, coordenada_t &celda, Matriz* matriz);
    
    //
    Animacion *ver_animacion(int tipo, int color);

    //
    Textura *ver_textura(int tipo, int color);
      
  private:
    animacion_t ***animaciones;
    animacion_t *estrella;
    int cant_tipos;
    int cant_colores;
};

#endif // PRODUCTOS_H
