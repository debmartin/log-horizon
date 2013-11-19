#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "SDL/SDL.h"
#include "matriz.h"

class Explosion{
  public:
    // Constructor de clase
    Explosion();
    
    // Destructor de clase
    ~Explosion();
    
    // Carga la animacion a ser ejecutada
    void cargar_animacion(const std::string &path, Ventana *ventana);
    
    // Inserta la animacion asignada anteriormente, en la matriz en
    // la posicion ingresada por parametro
    void explotar(coordenada_t &celda, Matriz* tablero);
    
    // Devuelve true si se esta realizando una explosion en el tablero
    bool explosion_en_curso();
    
    // Actualiza la animacion de la explosion
    void animar();
    
    // Devuelve true si la explosion finalizo
    bool finalizada();
     
  private:
    Textura *textura;
    Animacion *animacion;
    bool en_curso;
};

#endif // ESPLOSION_H
