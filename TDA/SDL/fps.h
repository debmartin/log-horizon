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
    int OldTime;
    int LastTime;
    float SpeedFactor;
    int NumFrames;
    int Frames;
};
 
#endif
