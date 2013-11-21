#include "fps.h"
#include <iostream>
 
// Constructor de clase
FPS::FPS(){
    OldTime = 0;
    Frames = 60;
    NumFrames = 60;
}
 
// Actualiza la cantidad de frames contados en el ultimo segundo
void FPS::actualizar(){
  if(OldTime + 1000 < (unsigned int)SDL_GetTicks()){
    OldTime = SDL_GetTicks(); 
    NumFrames = Frames;    
    Frames = 0;
  }
  Frames++;
}
 
// Devuelve la cantidad de frames en el ultimo segundo
int FPS::ver_fps(){
  return NumFrames;
}

