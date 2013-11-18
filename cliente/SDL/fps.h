#ifndef FPS_H
#define FPS_H
 
#include <SDL2/SDL.h>
 
class FPS { 
  public:
    //
    FPS();
    
    //
    void actualizar();

    //
    int ver_fps();

    //
    float ver_factor_velocidad();

  private:
    unsigned int OldTime;
    unsigned int LastTime;
    float SpeedFactor;
    unsigned int NumFrames;
    unsigned int Frames;
};
 
#endif
