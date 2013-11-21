#ifndef FPS_H
#define FPS_H
 
#include <SDL2/SDL.h>
 
class FPS { 
  public:
    // Constructor de clase
    FPS();
    
    // Actualiza la cantidad de frames contados en el ultimo segundo
    void actualizar();

    // Devuelve la cantidad de frames en el ultimo segundo
    int ver_fps();

  private:
    unsigned int OldTime;
    unsigned int NumFrames;
    unsigned int Frames;
};
 
#endif
