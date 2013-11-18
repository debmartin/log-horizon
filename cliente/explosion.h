#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "SDL/SDL.h"
#include "matriz.h"

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
    bool finalizada();
    
    //
    coordenada_t ver_primera();
  
  private:
    Textura *textura;
    Animacion *animacion;
    bool en_curso;
};

#endif // ESPLOSION_H
