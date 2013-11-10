#include "fps.h"
#include <iostream>
 
//
FPS::FPS(){
    OldTime = 0;
    LastTime = 0;
    SpeedFactor = 0;
    Frames = 0;
    NumFrames = 60;
}
 
//
void FPS::actualizar(){
  if(OldTime + 1000 < SDL_GetTicks()){
    OldTime = SDL_GetTicks(); 
    NumFrames = Frames;    
    Frames = 0;
  }
  SpeedFactor = ((SDL_GetTicks() - LastTime) / 1000.0f) * 60.0f;
  LastTime = SDL_GetTicks();
  Frames++;
}
 
//
int FPS::ver_fps(){
  return NumFrames;
}
 
//
float FPS::ver_factor_velocidad(){
  return SpeedFactor;
}
