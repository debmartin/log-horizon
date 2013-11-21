#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#include "../libs/SDL2/SDL.h"
#include "matriz.h"

typedef struct animacion_textura{
  Animacion *animacion;
  Textura *textura;
}animacion_t;

class Productos{
  public:
    // Constructor de clase
    Productos();
    
    // Destructor de clase
    ~Productos();
    
    // Carga Todos los productos de cada tipo y color en la carpeta del nivel
    // ingresado. Los archivos abiertos son: rojo.png, amarillo.png, verde.png,
    // azul.png, violeta.png y estrella.png
    // Pre: todos los archivos deben existir.
    // Post: Se Crearon todas las animaciones correspondientes a cada tipo
    //       y color de producto.
    void cargar_animaciones(const std::string &path, Ventana *ventana);
      
    // Actualiza todas las animaciones almacenadas.
    void animar();
    
    // Devuelve la animacion correspondiente al tipo y color ingresado.
    // en caso de no existir una animacion de dicho tipo o color, devuelve NULL
    Animacion *ver_animacion(int tipo, int color);

    // Devuelve la textura correspondiente al tipo y color ingresado
    // en caso de no existir una textura de dicho tipo o color, devuelve NULL
    Textura *ver_textura(int tipo, int color);
      
  private:
    animacion_t ***animaciones;
    animacion_t *estrella;
    int cant_tipos;
    int cant_colores;
};

#endif // PRODUCTOS_H
